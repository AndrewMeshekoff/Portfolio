#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "SDL/SDL.h"
#include "barrier.h"
#include "vec2D.h"
#include "object.h"

class player: public object{
    private:
    double w;
    double h;

    double xWalkAcc;
    double yWalkAcc;
    double xWalkVel;
    double yWalkVel;
    double xNetVel;
    double yNetVel;

    bool keyState[4];
    int xCurr;
    int yCurr;

    void updateFrame();
    void walkDecay();
    int frame;
    int walkDist;
    SDL_Rect Frame;
    SDL_Surface* sprite;

    public:
    friend class dot;

    player(std::string);
    ~player();
    void input(SDL_Event action);
    bool checkCollision(barrier*);
    bool collided;
    vec2D getNormal(barrier*);
    void bounce(vec2D);
    void block(vec2D);
    void updatePos();
    void drawPlayer();
};

#endif

