#include <SFML/Audio.h>
#include <SFML/Graphics.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#ifndef MY_H
#define MY_H

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define MAPWIDTH 24
#define MAPHEIGHT 24

#define TEXWIDTH 64
#define TEXHEIGHT 64

#define WIDTH  1600
#define HEIGHT 1200

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define RAD(angleDegrees) (angleDegrees * M_PI / 180.0)
#define DEG(angleRadians) (angleRadians * 180.0 / M_PI)
#define ABS(a) (((a) < 0) ? -(a) : (a))

sfVector2i G_MAP_COO;

static int map[MAPWIDTH][MAPHEIGHT] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 1, 1, 1, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 4, 3, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

typedef struct s_ray{
  // starting position of the ray (=== position of the player)
  sfVector2f pos;
  // direction of the ray
  sfVector2f dir;
  // length of the ray to the first edge
  sfVector2f sideDist;
  // length for the ray to reach an edge (from x and from y)
  sfVector2f deltaDist;
  // the coo on the map corresponding to the wall hit
  sfVector2i map;
  // length of a step between iterations
  sfVector2i step;
  // the exact wall coo (to texture the wall)
  sfVector2f wall;
  // the floor coo
  sfVector2f floor;
  //heigth of the wall to draw
  float line_h;
  // perpendicular distance to the wall (from x or y dpeending on the side which hit the wall)
  float perpWallDist;
  // side of thr hitten wall
  int side;
}t_ray;

/* The vector */
typedef struct{
        sfVector2f pos;
        sfVector2f dir;
}vector;

typedef struct s_my_framebuffer
{
  sfUint8 *pixels;
  int width;
  int height;
}t_my_framebuffer;

//float raycast(sfVector2f pos, float direction, int **map, sfVector2i mapSize)
void raycast(t_my_framebuffer *, sfVector2f, float);
void  putpixel(t_my_framebuffer *, int, int, sfColor);
void  drawline(t_my_framebuffer *, sfVector2i, sfVector2i, sfColor);
void  drawvert(t_my_framebuffer *, int, t_ray, t_my_framebuffer **);
t_my_framebuffer *create_pixel_buffer(int , int);
t_ray getRay(int, sfVector2f, float);
void raycast(t_my_framebuffer *, sfVector2f, float);
t_ray createRay(int, sfVector2f, float);
t_ray getDirection(t_ray);
t_ray dda(t_ray);
void drawFloor(t_my_framebuffer *, int, t_ray, t_my_framebuffer **);

// sfVector2f      move_forward(sfVector2f, float, float);

#endif
