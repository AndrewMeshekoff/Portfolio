#include "barrier.h"

barrier:: barrier(int xPos, int yPos, int width, int height, int elasticity)
: xPos(xPos), yPos(yPos), w(width), h(height), elasticity(elasticity)
{
draw_box.x = xPos;
draw_box.y = yPos;
draw_box.w = w;
draw_box.h = h;
}
