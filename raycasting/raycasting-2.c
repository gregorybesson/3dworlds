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

// typedef unsigned long long int64;

// static __inline__ int64 getTicks(void)
// {
//      unsigned a, d;
//      __asm volatile("rdtsc" : "=a" (a), "=d" (d));
//      return (((int64)a) | (((int64)d) << 32));
// }

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

void draw_map(sfUint8* pixels, double posX, double posY, double dirX, double dirY, double planeX, double planeY)
{
  int w = WIDTH;
  int h = HEIGHT;

  for(int x = 0; x < w; x++)
  {
    //calculate ray position and direction
    double cameraX = 2 * x / (double) w - 1; //x-coordinate in camera space
    double rayPosX = posX;
    double rayPosY = posY;
    double rayDirX = dirX + planeX * cameraX;
    double rayDirY = dirY + planeY * cameraX;
    //which box of the map we're in
    int mapX = (int)rayPosX;
    int mapY = (int)rayPosY;

    //length of ray from current position to next x or y-side
    double sideDistX;
    double sideDistY;

     //length of ray from one x or y-side to next x or y-side
    double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
    double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
    double perpWallDist;

    //what direction to step in x or y-direction (either +1 or -1)
    int stepX;
    int stepY;

    int hit = 0; //was there a wall hit?
    int side; //was a NS or a EW wall hit?
    //calculate step and initial sideDist
    if (rayDirX < 0)
    {
      stepX = -1;
      sideDistX = (rayPosX - mapX) * deltaDistX;
    }
    else
    {
      stepX = 1;
      sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
    }
    if (rayDirY < 0)
    {
      stepY = -1;
      sideDistY = (rayPosY - mapY) * deltaDistY;
    }
    else
    {
      stepY = 1;
      sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
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
    if (side == 0) perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
    else           perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

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

  // double time = 0; //time of current frame
  // double oldTime = 0; //time of previous frame

  //x and y start position
  double posX = 22;
  double posY = 12;

  //initial direction vector
  double dirX = -1;
  double dirY = 0;

  //the 2d raycaster version of camera plane
  double planeX = 0;
  double planeY = 0.66;

  window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
  sprite = sfSprite_create();
  texture = sfTexture_create(WIDTH, HEIGHT);
  pixels = create_pixel_buffer(WIDTH, HEIGHT);
  sfSprite_setTexture(sprite, texture, sfTrue);
  draw_map(pixels, posX, posY, dirX, dirY, planeX, planeY);
  sfTexture_updateFromPixels(texture, pixels, WIDTH, HEIGHT, 0, 0);

  if (!window)
      return EXIT_FAILURE;

  while (sfRenderWindow_isOpen(window))
  {
    // oldTime = time;
    // time = getTicks();
    // double frameTime = (time - oldTime) / 1000.0; //frameTime is the time this frame has taken, in seconds
    //print(1.0 / frameTime); //FPS counter

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
            if(worldMap[(int) (posX + dirX * moveSpeed)][(int)posY] == false) posX += dirX * moveSpeed;
            if(worldMap[(int) posX][(int)(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
          }
          //move backwards if no wall behind you
          if (event.key.code == sfKeyDown)
          {
            if(worldMap[(int) (posX - dirX * moveSpeed)][(int)posY] == false) posX -= dirX * moveSpeed;
            if(worldMap[(int) posX][(int)(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
          }
          //rotate to the right
          if (event.key.code == sfKeyRight)
          {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
          }
          //rotate to the left
          if (event.key.code == sfKeyLeft)
          {
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
          }   
        }
    }

    sfRenderWindow_clear(window, sfBlack);
    sfRenderWindow_drawSprite(window, sprite, NULL);
    sfRenderWindow_display(window);
    
    draw_map(pixels, posX, posY, dirX, dirY, planeX, planeY);
    sfTexture_updateFromPixels(texture, pixels, WIDTH, HEIGHT, 0, 0);
  }

  sfSprite_destroy(sprite);
  sfTexture_destroy(texture);
  sfRenderWindow_destroy(window);
  return EXIT_SUCCESS;
}
