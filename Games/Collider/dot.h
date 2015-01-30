#ifndef DOT_H
#define DOT_H

#include "SDL/SDL.h"
#include "vec2D.h"
#include "barrier.h"
#include "object.h"
#include "player.h"

class dot: public object{
    SDL_Surface* sprite;
    double radius;

    void bounce(player*, vec2D);
public:
    bool collided;

    dot(int, int);
    ~dot();
    void collide(barrier*);
    void collide(player*);
    vec2D getNormal(double, double);
    void bounce(vec2D);
    void collide(dot*);
    void updatePos();
    void drawDot();
};

#endif
