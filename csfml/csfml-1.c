#include <SFML/Graphics.h>
 
int main() {
    sfVideoMode mode = {200, 200, 32};
    sfRenderWindow * renderwindow;
    sfCircleShape * shape;
    shape = sfCircleShape_create();
    sfCircleShape_setRadius(shape, 100.f);
    renderwindow = sfRenderWindow_create(mode, "SFML works!", sfDefaultStyle ,NULL);
    sfCircleShape_setFillColor(shape, sfGreen);
 
    while (sfRenderWindow_isOpen(renderwindow))
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(renderwindow, &event))
        {
            if (event.type == sfEvtClosed)
            {
                sfRenderWindow_close(renderwindow);
            }
        }
 
        sfRenderWindow_clear(renderwindow, sfBlack);
        sfRenderWindow_drawCircleShape(renderwindow, shape, NULL);
        sfRenderWindow_display(renderwindow);
 
    }
 
    sfCircleShape_destroy(shape);
    sfRenderWindow_destroy(renderwindow);
 
    return 0;
}
