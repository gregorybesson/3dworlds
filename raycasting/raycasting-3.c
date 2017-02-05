/* A simple ray tracer */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h> /* Needed for boolean datatype */
#include <SFML/Graphics.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

/* Width and height of out image */
#define WIDTH  1600
#define HEIGHT 1200

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define min(a,b) (((a) < (b)) ? (a) : (b))

#define MAPWIDTH 24
#define MAPHEIGHT 24

/* The coordinate structure */
typedef struct{
      double x;
      double y;
}coo;

/* The vector */
typedef struct{
        coo pos;
        coo dir;
}vector;

sfUint8* create_pixel_buffer(int width, int height)
{
  int i;
  sfUint8* pixels;

  pixels = malloc(width * height * 4 * sizeof(*pixels));
  if (pixels == NULL)
  {
    exit(EXIT_FAILURE);
  }
  i = 0;
  while (i < width * height * 4)
  {
    pixels[i] = 0;
    i = i + 1;
  }
  return pixels;
}

int worldMap[MAPWIDTH][MAPHEIGHT]=
  {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
    {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
  };

void draw_map(sfUint8* pixels, vector player, coo plane)
{
  int w = WIDTH;
  int h = HEIGHT;

  for(int x = 0; x < w; x++)
  {
    //calculate ray position and direction
    double cameraX = 2 * x / (double) w - 1; //x-coordinate in camera space
    vector ray;
    ray.pos.x = player.pos.x;
    ray.pos.y = player.pos.y;
    ray.dir.x = player.dir.x + plane.x * cameraX;
    ray.dir.y = player.dir.y + plane.y * cameraX;

    //which box of the map we're in
    int mapX = (int)ray.pos.x;
    int mapY = (int)ray.pos.y;

    //length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;

     //length of ray from one x or y-side to next x or y-side
    double deltaDistX = sqrt(1 + (ray.dir.y * ray.dir.y) / (ray.dir.x * ray.dir.x));
    double deltaDistY = sqrt(1 + (ray.dir.x * ray.dir.x) / (ray.dir.y * ray.dir.y));
    double perpWallDist;

    //what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    int hit = 0; //was there a wall hit?
    int side; //was a NS or a EW wall hit?
    //calculate step and initial sideDist
    if (ray.dir.x < 0)
    {
      stepX = -1;
      sideDistX = (ray.pos.x - mapX) * deltaDistX;
    }
    else
    {
      stepX = 1;
      sideDistX = (mapX + 1.0 - ray.pos.x) * deltaDistX;
    }
    if (ray.dir.y < 0)
    {
      stepY = -1;
      sideDistY = (ray.pos.y - mapY) * deltaDistY;
    }
    else
    {
      stepY = 1;
      sideDistY = (mapY + 1.0 - ray.pos.y) * deltaDistY;
    }
    //perform DDA
    while (hit == 0)
    {
      //jump to next map square, OR in x-direction, OR in y-direction
      if (sideDistX < sideDistY)
      {
        sideDistX += deltaDistX;
        mapX += stepX;
        side = 0;
      }
      else
      {
        sideDistY += deltaDistY;
        mapY += stepY;
        side = 1;
      }
      //Check if ray has hit a wall
      if (worldMap[mapX][mapY] > 0) hit = 1;
    }
    //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
    if (side == 0) perpWallDist = (mapX - ray.pos.x + (1 - stepX) / 2) / ray.dir.x;
    else           perpWallDist = (mapY - ray.pos.y + (1 - stepY) / 2) / ray.dir.y;

    //Calculate height of line to draw on screen
    int lineHeight = (int)(h / perpWallDist);

    //calculate lowest and highest pixel to fill in current stripe
    int drawStart = -lineHeight / 2 + h / 2;
    if(drawStart < 0)drawStart = 0;
    int drawEnd = lineHeight / 2 + h / 2;
    if(drawEnd >= h)drawEnd = h - 1;

    //choose wall color
    sfColor color;
    switch(worldMap[mapX][mapY])
    {
      case 1:  
        color = sfRed;  
        break; //red
      case 2:  
        color = sfGreen;  
        break; //green
      case 3:  
        color = sfBlue;  
        break; //blue
      case 4:  
        color = sfYellow;  
        break; //white
      default: 
        color = sfMagenta;  
        break; //yellow
    }

    //give x and y sides different brightness
    if (side == 1)
    {
      color.r = (int) color.r / 2;
      color.g = (int) color.g / 2;
      color.b = (int) color.b / 2;
    }

    int y = 0;
    while (y < HEIGHT)
    {
      if(y < drawStart || y > drawEnd){
        pixels[(WIDTH * y + x) * 4] = 0;
        pixels[(WIDTH * y + x) * 4 + 1] = 0;
        pixels[(WIDTH * y + x) * 4 + 2] = 0;
        pixels[(WIDTH * y + x) * 4 + 3] = 0;
      } else {
        pixels[(WIDTH * y + x) * 4] = color.r;
        pixels[(WIDTH * y + x) * 4 + 1] = color.g;
        pixels[(WIDTH * y + x) * 4 + 2] = color.b;
        pixels[(WIDTH * y + x) * 4 + 3] = color.a;
      }
      y = y + 1;
    }
  }
}

int main(){
    
  sfVideoMode mode = {WIDTH, HEIGHT, 32};
  sfRenderWindow* window;
  sfTexture* texture;
  sfSprite* sprite;
  sfUint8* pixels;
  sfEvent event;

  vector player;
  coo plane;

  //x and y start position
  player.pos.x = 22;
  player.pos.y = 12;

  //initial direction coo
  player.dir.x = -1;
  player.dir.y = 0;

  //the 2d raycaster version of camera plane
  plane.x = 0;
  plane.y = 0.66;

  window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
  sprite = sfSprite_create();
  texture = sfTexture_create(WIDTH, HEIGHT);
  pixels = create_pixel_buffer(WIDTH, HEIGHT);
  sfSprite_setTexture(sprite, texture, sfTrue);
  draw_map(pixels, player, plane);
  sfTexture_updateFromPixels(texture, pixels, WIDTH, HEIGHT, 0, 0);

  if (!window)
      return EXIT_FAILURE;

  while (sfRenderWindow_isOpen(window))
  {
    //speed modifiers
    double moveSpeed = .3;//frameTime * 5.0; //the constant value is in squares/second
    double rotSpeed = .1;//frameTime * 3.0; //the constant value is in radians/second

    while (sfRenderWindow_pollEvent(window, &event))
    {
        if (event.type == sfEvtClosed)
          sfRenderWindow_close(window);
        if (event.type == sfEvtKeyPressed ){
          if (event.key.code == sfKeyEscape){
            sfRenderWindow_close(window);
          }

          if (event.key.code == sfKeyUp)
          {
            if(worldMap[(int) (player.pos.x + player.dir.x * moveSpeed)][(int)player.pos.y] == false) player.pos.x += player.dir.x * moveSpeed;
            if(worldMap[(int) player.pos.x][(int)(player.pos.y + player.dir.y * moveSpeed)] == false) player.pos.y += player.dir.y * moveSpeed;
          }
          //move backwards if no wall behind you
          if (event.key.code == sfKeyDown)
          {
            if(worldMap[(int) (player.pos.x - player.dir.x * moveSpeed)][(int)player.pos.y] == false) player.pos.x -= player.dir.x * moveSpeed;
            if(worldMap[(int) player.pos.x][(int)(player.pos.y - player.dir.y * moveSpeed)] == false) player.pos.y -= player.dir.y * moveSpeed;
          }
          //rotate to the right
          if (event.key.code == sfKeyRight)
          {
            //both camera direction and camera plane must be rotated
            coo oldDir = player.dir;
            player.dir.x = player.dir.x * cos(-rotSpeed) - player.dir.y * sin(-rotSpeed);
            player.dir.y = oldDir.x * sin(-rotSpeed) + player.dir.y * cos(-rotSpeed);
            coo oldPlane = plane;
            plane.x = plane.x * cos(-rotSpeed) - plane.y * sin(-rotSpeed);
            plane.y = oldPlane.x * sin(-rotSpeed) + plane.y * cos(-rotSpeed);
          }
          //rotate to the left
          if (event.key.code == sfKeyLeft)
          {
            //both camera direction and camera plane must be rotated
            coo oldDir = player.dir;
            player.dir.x = player.dir.x * cos(rotSpeed) - player.dir.y * sin(rotSpeed);
            player.dir.y = oldDir.x * sin(rotSpeed) + player.dir.y * cos(rotSpeed);
            coo oldPlane = plane;
            plane.x = plane.x * cos(rotSpeed) - plane.y * sin(rotSpeed);
            plane.y = oldPlane.x * sin(rotSpeed) + plane.y * cos(rotSpeed);
          }   
        }
    }

    sfRenderWindow_clear(window, sfBlack);
    sfRenderWindow_drawSprite(window, sprite, NULL);
    sfRenderWindow_display(window);
    
    draw_map(pixels, player, plane);
    sfTexture_updateFromPixels(texture, pixels, WIDTH, HEIGHT, 0, 0);
  }

  sfSprite_destroy(sprite);
  sfTexture_destroy(texture);
  sfRenderWindow_destroy(window);
  return EXIT_SUCCESS;
}
