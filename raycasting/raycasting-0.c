#include <math.h>
 
sfVector2f  move_forward(sfVector2f pos, float direction, float distance)
{
  pos.x += distance * cos(direction * acos(-1) / 180);
  pos.y += distance * sin(direction * acos(-1) / 180);
  return (pos);
}
 
float raycast(sfVector2f pos, float direction, int **map, sfVector2i mapSize)
{
  float distance;
  float x;
  float y;
 
  x = pos.x;
  y = pos.y;
  while (map[(int)pos.x][(int)pos.y] == 0 && pos.x <= mapSize.x && pos.y <= mapSize.y)
    pos = move_forward(pos, direction, 1);
    distance = sqrt(pow(pos.x - x, 2) + pow(pos.y - y, 2));
  return (distance);
}
