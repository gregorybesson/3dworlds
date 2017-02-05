#include "raycast.h"

t_ray createRay(int x, sfVector2f pos, float direction){
  sfVector2f plane;
  t_ray ray;
  float cameraX;

  cameraX = 2 * x / (float) WIDTH - 1;
  ray.pos.x = pos.x;
  ray.pos.y = pos.y;
  plane.x = sin(direction*acos(-1)/180);
  plane.y = -cos(direction*acos(-1)/180);
  ray.dir.x = cos(direction*acos(-1)/180) + plane.x * cameraX;
  ray.dir.y = sin(direction*acos(-1)/180) + plane.y * cameraX;
  ray.map.x = (int)ray.pos.x;
  ray.map.y = (int)ray.pos.y;
  ray.deltaDist.x = sqrt(1 + (ray.dir.y * ray.dir.y) / (ray.dir.x * ray.dir.x));
  ray.deltaDist.y = sqrt(1 + (ray.dir.x * ray.dir.x) / (ray.dir.y * ray.dir.y));

  return (ray);

}

t_ray getDirection(t_ray ray){
  if (ray.dir.x < 0)
  {
    ray.step.x = -1;
    ray.sideDist.x = (ray.pos.x - ray.map.x) * ray.deltaDist.x;
  }
  else
  {
    ray.step.x = 1;
    ray.sideDist.x = (ray.map.x + 1.0 - ray.pos.x) * ray.deltaDist.x;
  }
  if (ray.dir.y < 0)
  {
    ray.step.y = -1;
    ray.sideDist.y = (ray.pos.y - ray.map.y) * ray.deltaDist.y;
  }
  else
  {
    ray.step.y = 1;
    ray.sideDist.y = (ray.map.y + 1.0 - ray.pos.y) * ray.deltaDist.y;
  }

  return (ray);
}

t_ray dda(t_ray ray){
  while (map[ray.map.x][ray.map.y] == 0)
    {
      if (ray.sideDist.x < ray.sideDist.y)
      {
        ray.sideDist.x += ray.deltaDist.x;
        ray.map.x += ray.step.x;
        ray.side = 0;
      }
      else
      {
        ray.sideDist.y += ray.deltaDist.y;
        ray.map.y += ray.step.y;
        ray.side = 1;
      }
    }

  return (ray);
}
//float raycast(sfVector2f pos, float direction, int **map, sfVector2i mapSize)
void raycast(t_my_framebuffer *framebuffer, sfVector2f pos, float direction)
{

  t_my_framebuffer *texture[8];

  for(int i = 0; i < 8; i++) texture[i] = create_pixel_buffer(WIDTH, HEIGHT);
  for(int x = 0; x < TEXWIDTH; x++)
  {
    for(int y = 0; y < TEXHEIGHT; y++)
    {
      int xorcolor = (x * 256 / TEXWIDTH) ^ (y * 256 / TEXHEIGHT);
      //int xcolor = x * 256 / TEXWIDTH;
      int ycolor = y * 256 / TEXHEIGHT;
      int xycolor = y * 128 / TEXHEIGHT + x * 128 / TEXWIDTH;
      texture[0]->pixels[TEXWIDTH * y + x] = 65536 * 254 * (x != y && x != TEXWIDTH - y); //flat red texture with black cross
      texture[1]->pixels[TEXWIDTH * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
      texture[2]->pixels[TEXWIDTH * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
      texture[3]->pixels[TEXWIDTH * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
      texture[4]->pixels[TEXWIDTH * y + x] = 256 * xorcolor; //xor green
      texture[5]->pixels[TEXWIDTH * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
      texture[6]->pixels[TEXWIDTH * y + x] = 65536 * ycolor; //red gradient
      texture[7]->pixels[TEXWIDTH * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
    }
  }

  for(int x = 0; x < WIDTH; x++)
  {
    t_ray ray;
    int hit = 0;

    ray = createRay(x, pos, direction);
    ray = getDirection(ray);
    ray = dda(ray);

    if (ray.side == 0){
      ray.perpWallDist = (ray.map.x - ray.pos.x + (1 - ray.step.x) / 2) / ray.dir.x;
      ray.wall.x = ray.pos.y + ray.perpWallDist * ray.dir.y;
    } else {
      ray.perpWallDist = (ray.map.y - ray.pos.y + (1 - ray.step.y) / 2) / ray.dir.y;
      ray.wall.x = ray.pos.x + ray.perpWallDist * ray.dir.x;
    }

    ray.wall.x -= floor(ray.wall.x);
    ray.line_h = (HEIGHT / ray.perpWallDist);
    drawvert(framebuffer, x, ray, texture);
    drawFloor(framebuffer, x, ray, texture);
  }
}

void drawFloor(t_my_framebuffer *framebuffer, int x, t_ray ray, t_my_framebuffer **texture){

  float drawEnd;

  if(ray.side == 0 && ray.dir.x > 0)
  {
    ray.floor.x = ray.map.x;
    ray.floor.y = ray.map.y + ray.wall.x;
  }
  else if(ray.side == 0 && ray.dir.x < 0)
  {
    ray.floor.x = ray.map.x + 1.0;
    ray.floor.y = ray.map.y + ray.wall.x;
  }
  else if(ray.side == 1 && ray.dir.y > 0)
  {
    ray.floor.x = ray.map.x + ray.wall.x;
    ray.floor.y = ray.map.y;
  }
  else
  {
    ray.floor.x = ray.map.x + ray.wall.x;
    ray.floor.y = ray.map.y + 1.0;
  }

  float currentDist;

  drawEnd = HEIGHT / 2 + ray.line_h / 2;
  if (drawEnd < 0) drawEnd = HEIGHT;

  for(int y = drawEnd + 1; y < HEIGHT; y++)
  {
    currentDist = HEIGHT / (2.0 * y - HEIGHT);

    float weight = currentDist / ray.perpWallDist;

    double currentFloorX = weight * ray.floor.x + (1.0 - weight) * ray.pos.x;
    double currentFloorY = weight * ray.floor.y + (1.0 - weight) * ray.pos.y;

    int floorTexX, floorTexY;
    floorTexX = (int) (currentFloorX * TEXWIDTH) % TEXWIDTH;
    floorTexY = (int) (currentFloorY * TEXHEIGHT) % TEXHEIGHT;

    //floor
    framebuffer->pixels[(WIDTH * y + x)*4] = (texture[3]->pixels[TEXWIDTH * floorTexY + floorTexX] >> 1) & 8355711;
    //ceiling (symmetrical!)
    framebuffer->pixels[(WIDTH * (HEIGHT - y) + x)*4] = texture[1]->pixels[TEXWIDTH * floorTexY + floorTexX];
  }
}
