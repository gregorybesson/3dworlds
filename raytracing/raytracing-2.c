/* A simple ray tracer */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h> /* Needed for boolean datatype */
#include <SFML/Graphics.h>

/* Width and height of out image */
#define WIDTH  1024
#define HEIGHT 768

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

/* The vector structure */
typedef struct{
      float x,y,z;
}vector;

/* The sphere */
typedef struct{
        vector pos;
        float  radius;
}sphere; 

/* The ray */
typedef struct{
        vector start;
        vector dir;
}ray;

/* Subtract two vectors and return the resulting vector */
vector vectorSub(vector *v1, vector *v2){
    vector result = {v1->x - v2->x, v1->y - v2->y, v1->z - v2->z };
    return result;
}

/* Multiply two vectors and return the resulting scalar (dot product) */
float vectorDot(vector *v1, vector *v2){
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}


/* Check if the ray and sphere intersect */
bool intersectRaySphere(ray *r, sphere *s){
    
    /* A = d.d, the vector dot product of the direction */
    float A = vectorDot(&r->dir, &r->dir); 
    
    /* We need a vector representing the distance between the start of 
     * the ray and the position of the circle.
     * This is the term (p0 - c) 
     */
    vector dist = vectorSub(&r->start, &s->pos);
    
    /* 2d.(p0 - c) */  
    float B = 2 * vectorDot(&r->dir, &dist);
    
    /* (p0 - c).(p0 - c) - r^2 */
    float C = vectorDot(&dist, &dist) - (s->radius * s->radius);
    
    /* Solving the discriminant */
    float discr = B * B - 4 * A * C;
    
    /* If the discriminant is negative, there are no real roots.
     * Return false in that case as the ray misses the sphere.
     * Return true in all other cases (can be one or two intersections)
     */
    if(discr < 0)
        return false;
    else
        return true;
}

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

void raytrace(sfUint8* pixels) {
  /* Our ray and a sphere */
  sphere s;
  ray r;
  
  /* x, y for screen 'resolution' */
  int x,y;    

  /* Intersect ray/sphere or not */
  bool hit;

  /* Position the sphere */
  s.pos.x = 200;
  s.pos.y = 200;
  s.pos.z = 100;

  /* Sphere radius */
  s.radius = 100;

  /* Direction of the ray */
  r.dir.x = 0;
  r.dir.y = 0;
  r.dir.z = 1;

  /* Start position of the ray, z coordinate */
  r.start.z = 0;

  /* Iterate over every pixel of our screen */
  for(y=0;y<HEIGHT;y++)
  {
    /* Set the y-coordinate of the start position of the ray */
    r.start.y = y; 
    for(x=0;x<WIDTH;x++)
    {
      /* Set the x-coordinate of the start position of the ray */
      r.start.x = x;

      /* Check if the ray intersects with the shpere */
      hit = intersectRaySphere(&r, &s);
      if(hit)
      {
        pixels[(x + y*WIDTH)*4 + 0] = sfRed.r;
        pixels[(x + y*WIDTH)*4 + 1] = sfRed.g;
        pixels[(x + y*WIDTH)*4 + 2] = sfRed.b;
        pixels[(x + y*WIDTH)*4 + 3] = sfRed.a;
      } 
      else 
      {
        pixels[(x + y*WIDTH)*4 + 0] = 0;
        pixels[(x + y*WIDTH)*4 + 1] = 0;
        pixels[(x + y*WIDTH)*4 + 2] = 0;
        pixels[(x + y*WIDTH)*4 + 3] = 0;
      }
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

  window = sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL);
  sprite = sfSprite_create();
  texture = sfTexture_create(WIDTH, HEIGHT);
  pixels = create_pixel_buffer(WIDTH, HEIGHT);
  sfSprite_setTexture(sprite, texture, sfTrue);
  raytrace(pixels);
  sfTexture_updateFromPixels(texture, pixels, WIDTH, HEIGHT, 0, 0);

  if (!window)
      return EXIT_FAILURE;

  while (sfRenderWindow_isOpen(window))
  {
    while (sfRenderWindow_pollEvent(window, &event))
    {
        if (event.type == sfEvtClosed)
            sfRenderWindow_close(window);
    }

    sfRenderWindow_clear(window, sfBlack);
    sfRenderWindow_drawSprite(window, sprite, NULL);
    sfRenderWindow_display(window);
  }

  sfSprite_destroy(sprite);
  sfTexture_destroy(texture);
  sfRenderWindow_destroy(window);
  return EXIT_SUCCESS;
}
