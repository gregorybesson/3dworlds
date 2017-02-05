#include <SFML/Graphics.h>

#include <unistd.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

sfRenderWindow* create_window(char* name, int width, int height)
{
  sfRenderWindow* window;
  sfVideoMode mode;

  mode.width = width;
  mode.height = height;
  mode.bitsPerPixel = 32;
  window = sfRenderWindow_create(mode, name, sfResize | sfClose, NULL);
  if (window == NULL)
  {
    exit(EXIT_FAILURE);
  }
  return window;
}

void draw_square(sfUint8* pixels)
{
  int x;
  int y;

  x = 100;
  while (x < 200)
  {
    y = 100;
    while (y < 200)
    {
      pixels[(SCREEN_WIDTH * y + x) * 4] = sfRed.r;
      pixels[(SCREEN_WIDTH * y + x) * 4 + 1] = sfRed.g;
      pixels[(SCREEN_WIDTH * y + x) * 4 + 2] = sfRed.b;
      pixels[(SCREEN_WIDTH * y + x) * 4 + 3] = sfRed.a;
      y = y + 1;
    }
    x = x + 1;
  }
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

int main()
{
  sfRenderWindow* window;
  sfTexture* texture;
  sfSprite* sprite;
  sfUint8* pixels;
  sfEvent event;

  window = create_window("SFML Window", SCREEN_WIDTH, SCREEN_HEIGHT);
  sprite = sfSprite_create();
  texture = sfTexture_create(SCREEN_WIDTH, SCREEN_HEIGHT);
  pixels = create_pixel_buffer(SCREEN_WIDTH, SCREEN_HEIGHT);
  sfSprite_setTexture(sprite, texture, sfTrue);
  draw_square(pixels);
  sfTexture_updateFromPixels(texture, pixels, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

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
  sfRenderWindow_destroy(window);
  return (EXIT_SUCCESS);
}