#include "raycast.h"

t_my_framebuffer *create_pixel_buffer(int width, int height)
{
  int i;
  sfUint8* pixels;
  t_my_framebuffer *pixelbuffer;

  pixels = malloc(width * height * 4 * sizeof(*pixels));
  pixelbuffer = malloc(width * height * 4 * sizeof(*pixelbuffer));
  if (pixels == NULL)
    exit(EXIT_FAILURE);
  i = 0;
  while (i < width * height * 4)
    {
      pixels[i] = 0;
      i = i + 1;
    }
  pixelbuffer->pixels = pixels;
  pixelbuffer->height = height;
  pixelbuffer->width = width;
  return (pixelbuffer);
}

void  drawline(t_my_framebuffer *framebuffer, sfVector2i from, sfVector2i to, sfColor color)
{
  while (from.y < to.y)
    {
      putpixel(framebuffer, from.x, from.y, color);
      from.y++;
    }
}

void  drawvert(t_my_framebuffer *framebuffer, int x, t_ray ray, t_my_framebuffer **texture)
{
  int draw_start;
  int draw_end;
  sfVector2i  from;
  sfVector2i  to;
  sfColor color;
  int texNum = map[ray.map.x][ray.map.y] - 1;

  draw_start = HEIGHT / 2 - ray.line_h / 2;
  if (draw_start < 0)
    draw_start = 0;
  draw_end = HEIGHT / 2 + ray.line_h / 2;
  if (draw_end >= HEIGHT)
    draw_end = HEIGHT - 1;
  from.x = x;
  to.x = WIDTH;
  from.y = 0;
  to.y = draw_start;

  //drawline(framebuffer, from, to, sfCyan);
  from.y = draw_start;
  to.y = draw_end;
 
  if (map[G_MAP_COO.x][G_MAP_COO.y] == 1)
    color = sfRed;
  else if (map[G_MAP_COO.x][G_MAP_COO.y] >= 2)
    color = sfBlue;
  else
    color = sfWhite;
  drawline(framebuffer, from, to, color);
    //x coordinate on the texture
  int texX = (int)(ray.wall.x * TEXWIDTH);
  if(ray.side == 0 && ray.dir.x > 0) texX = TEXWIDTH - texX - 1;
  if(ray.side == 1 && ray.dir.y < 0) texX = TEXWIDTH - texX - 1;

  for(int y = draw_start; y < draw_end; y++)
  {
    int d = y * 256 - HEIGHT * 128 + ray.line_h * 128;  //256 and 128 factors to avoid floats
    int texY = ((d * TEXHEIGHT) / ray.line_h) / 256;

    if(ray.side == 0){
      framebuffer->pixels[(WIDTH * y + x) * 4] = texture[2]->pixels[TEXHEIGHT * 10 + 1];
    } else {
      framebuffer->pixels[(WIDTH * y + x) * 4] = (texture[4]->pixels[TEXHEIGHT * 10 + 1] >> 1) & 8355711;
    }
  }
  color = sfGreen;
  from.y = draw_end;
  to.y = HEIGHT;
  //drawline(framebuffer, from, to, sfGreen);
}

void  putpixel(t_my_framebuffer *framebuffer, int x, int y, sfColor color)
{
  framebuffer->pixels[(WIDTH * y + x) * 4] = color.r;
  framebuffer->pixels[(WIDTH * y + x) * 4 + 1] = color.g;
  framebuffer->pixels[(WIDTH * y + x) * 4 + 2] = color.b;
  framebuffer->pixels[(WIDTH * y + x) * 4 + 3] = color.a;
}

