#include "SDL/SDL.h"

#ifndef BARRIER_H
#define BARRIER_H

struct barrier{
    int xPos;
    int yPos;

    int w;
    int h;

    SDL_Rect draw_box;

    int elasticity;
    barrier(int, int, int, int, int);
};

#endif
