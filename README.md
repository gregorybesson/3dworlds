# 3dworlds
Raycasting + Raytracing in C

# Introduction

# Creating a World

- Create a 2D array of 0 and integers. Integers will represent different kinds of walls.

111111111111

100000000001

100000000001

100222000001

100000003331

111020000001

100020000001

100020000001

# Defining Projection Attributes
The screen will be my camera plan. The trick here is that I will parse each x pixel of the screen, looking for 
an intersection with a wall. then, I'll create the resulting vertical.

I need my player object, with position (x, y) and the direction he goes (a NORMALIZED vector)
I need my camera plan. Another trick: I want the angle of the camera to be 66° for the game to be "realistic". Thanks to CAH SOH TOA trigonometric mnemonic, I can deduce the distance between the camera plan and the player.

# Finding Distance To Projection Plane
Now I will project rays from player position to the camera plan. I need the direction vector of each ray to be able to ray to the walls.
As I'll express the x coordinates based on the real screen size, I need to apply a ratio to it as all my calculations occur in the grid scale. Furthermore, I want the x center of my screen to be the x = 0. my camera plan starting at -1 and finishing at 1. See below the cameraRatio formula.
to calculate the ray dir  NORMALIZED vector, we add the player dir normalized vector with the norm of the part normalized vector of the x on the camera plan. in cartesian coo.

Calculations:
```
pointVector player;
player.pos.x = 3;
player.pos.y = 3;
player.dir.x = 0;
player.dir.y = -1;

vector cameraPlan;
cameraPlan.x = 1;
cameraPlan.y = 0.66

for(int i=0; i < SCREEN_WIDTH. i++){
  long cameraRatio = (2 * x) / SCREEN_WIDTH - 1;
  pointVector ray;
  ray.pos.x = player.pos.x;
  ray.pos.y = player.pos.y;
  ray.dir.x = player.dir.x + cameraPlan.x X cameraRatio;
  ray.dir.y = player.dir.y + cameraPlan.y X cameraRatio;
}

```

# Finding Walls
if we have a screen: SCREEN_WIDTH=1024, SCREEN_HEIGHT=768
the previous calculation has been done for 1024 rays. Now it's time to find walls. Here comes the DDA algorithm 
(Digital Differential Algorithm)

## Finding the horizontal intersections
We want to calculate the initial length of the y component of ray normalized dir vector: sideDistY
We want to calculate the distance between 2 horizontal lines following the ray normalized dir vector: deltaDistY
This value is a constant.
It is calculated thanks to Pythagore's theorem: deltaDistY² = 1² + length²

ray dir is a normalized vector. We want its y component to equal 1. So we have:
1 = ray.dir.y * 1/ray.dir.x
1 / ray.dir.y being the multiplier to make the y vector component of ray = 1
We have then: ray.dir.x * 1/ray.dir.y as we multiply both components vector to keep the same direction.

we deduce:
deltaDistY² = 1² + (ray.dir.x * 1/ray.dir.y)²

## Finding the vertical intersections
We do apply the same on vertical intersections. This time, we want to normalize ray.dir.x:
1 = ray.dir.x * 1/ray.dir.x
we then have the new y component of the vector:
ray.dir.y X 1/ray.dir.x

And Pythagore being applied:
deltaDistX² = (ray.dir.y X 1/ray.dir.x)² + 1

## The wall

# Finding Distance to Walls
# Drawing Walls
# Texture Mapped Walls
# Drawing Floors
# Drawing Ceilings
# Walls with Variable Height
# Horizontal Motion /  / Making the Player Move
# Looking Up and Down
# Flying and Crouching
# Combined Effects
# Shading

# Bibliography
