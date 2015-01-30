#include <cstdlib>
#include <string>
#include <cmath>
#include "SDL/SDL.h"
#include "dot.h"
#include "barrier.h"
#include "vec2D.h"

extern SDL_Surface* screen;

SDL_Surface* Load_image(std::string);
void color_key(SDL_Surface* image, int R, int G, int B);

dot::dot(int x, int y): object(1.0){
    xPos = x;
    yPos = y;
    radius = 10.0;

    collided = false;

    sprite = Load_image("dot.bmp");
    color_key(sprite, 255, 255, 255);
}

dot::~dot(){
    SDL_FreeSurface(sprite);
}

vec2D dot::getNormal(double x, double y){
    vec2D norm;
    norm.x =(xPos-x);
    norm.y =(yPos-y);
    return norm;
}

void dot::bounce(vec2D norm){
    vec2D vel;
    vel.x=xVel;
    vel.y=yVel;
    vec2D per = norm.scalProd(vel.dotProd(norm)/norm.dotProd(norm));
    vec2D par = vel-per;
    vel = par-per;
    xVel = vel.x;
    yVel = vel.y;
}

void dot::collide(dot* impact){
    if(distance(impact)<=(2*radius)){
        vec2D inorm = getNormal(impact->xPos, impact->yPos);
        vec2D ivel;
        ivel.x=xVel;
        ivel.y=yVel;
        vec2D iper = inorm.scalProd(ivel.dotProd(inorm)/inorm.dotProd(inorm));
        vec2D ipar = ivel-iper;

//        vec2D enorm = impact->getNormal(xPos, yPos);
        vec2D enorm = inorm.scalProd(-1);
        vec2D evel;
        evel.x=impact->xVel;
        evel.y=impact->yVel;
        vec2D eper = enorm.scalProd(evel.dotProd(enorm)/enorm.dotProd(enorm));
        vec2D epar = evel-eper;

        vec2D ifper = iper.scalProd((mass-(impact->mass))/(mass+(impact->mass)))+eper.scalProd(2*(impact->mass)/(mass+(impact->mass)));
//        vec2D efper = iper.scalProd(2*mass/(mass+(impact->mass)))+eper.scalProd(((impact->mass)-mass)/(mass+(impact->mass)));
        vec2D efper = iper + ifper - eper;

        ivel = ipar + ifper;
        evel = epar + efper;

        xVel = ivel.x;
        yVel = ivel.y;

        impact->xVel = evel.x;
        impact->yVel = evel.y;

        collided = true;
    }
}

void dot::collide(barrier* box){
    double xClos, yClos;

    if((xPos>box->xPos)&&(xPos<(box->xPos+box->w)))
        xClos = xPos;
    else if(xPos<=box->xPos)
        xClos = box->xPos;
    else
        xClos = (box->xPos + box->w);

    if((yPos>box->yPos)&&(yPos<(box->yPos+box->h)))
        yClos = yPos;
    else if(yPos<=box->yPos)
        yClos = box->yPos;
    else
        yClos = (box->yPos + box->h);

    if(distance(xClos, yClos)<=radius){
        double xvTemp=xVel;
        double yvTemp=yVel;
        bounce(getNormal(xClos, yClos));
        xPos-=xvTemp;
        yPos-=yvTemp;
    }
}

void dot::collide(player* move){
    double xClos, yClos;
    double charLeft = move->xPos-move->w/2;
    double charRight = move->xPos+move->w/2;
    double charUp = move->yPos-move->h/2;
    double charDown = move->yPos+move->h/2;

    if((xPos>charLeft)&&(xPos<charRight))
        xClos = xPos;
    else if(xPos<=charLeft)
        xClos = charLeft;
    else
        xClos = charRight;

    if((yPos>charUp)&&(yPos<charDown))
        yClos = yPos;
    else if(yPos<=charUp)
        yClos = charUp;
    else
        yClos = charDown;

    if(distance(xClos, yClos)<=radius){
        move->collided=true;
//        if(move->collidable){
        bounce(move, getNormal(xClos, yClos));
        move->block(getNormal(xClos, yClos).scalProd(-1));
//        move->collidable=false;
//        }
    }
}

void dot::bounce(player* impact, vec2D inorm){
    double walkVel = impact->xWalkVel + impact->yWalkVel;
    double physVel = impact->xVel + impact->yVel;

    vec2D ivel;
    ivel.x=xVel;
    ivel.y=yVel;
    vec2D iper = inorm.scalProd(ivel.dotProd(inorm)/inorm.dotProd(inorm));
    vec2D ipar = ivel-iper;

    vec2D enorm = inorm.scalProd(-1);
    vec2D evel;
    evel.x = impact->xVel + impact->xWalkVel;
    evel.y = impact->yVel + impact->yWalkVel;
    vec2D eper = enorm.scalProd(evel.dotProd(enorm)/enorm.dotProd(enorm));
    vec2D epar = evel-eper;

    vec2D ifper = iper.scalProd((mass-(impact->mass))/(mass+(impact->mass)))+eper.scalProd(2*(impact->mass)/(mass+(impact->mass)));
//    vec2D efper = iper.scalProd(2*mass/(mass+(impact->mass)))+eper.scalProd(((impact->mass)-mass)/(mass+(impact->mass)));
    vec2D efper = iper + ifper - eper;

// Test
//    vec2D implicit;
//
//    evel.x=impact->xVel;
//    evel.y=impact->yVel;
//    eper = enorm.scalProd(evel.dotProd(enorm)/enorm.dotProd(enorm));
//    vec2D epar = evel-eper;
//
//    if(physVel!=0)
//        implicit = iper.scalProd(2*mass/(mass+(impact->mass))*(physVel/(walkVel+physVel)));
//    else if(walkVel==0&&physVel==0)
//        implicit = iper.scalProd(2*mass/(mass+(impact->mass)));
//    else{
//        implicit.x = 0;
//        implicit.y = 0;
//    }
//
//    vec2D efper = implicit + eper.scalProd(((impact->mass)-mass)/(mass+(impact->mass)));
//
//    evel.x=impact->xWalkVel;
//    evel.y=impact->yWalkVel;
//    eper = enorm.scalProd(evel.dotProd(enorm)/enorm.dotProd(enorm));
//    vec2D ewpar = evel-eper;
//
//    if(walkVel!=0)
//        implicit = iper.scalProd(2*mass/(mass+(impact->mass))*(walkVel/(walkVel+physVel)));
//    else{
//        implicit.x = 0;
//        implicit.y = 0;
//    }
//
//    vec2D efwper = implicit + eper.scalProd(((impact->mass)-mass)/(mass+(impact->mass)));

    ivel = ipar + ifper;
    evel = epar + efper;
//    vec2D ewvel = ewpar + efwper;

    xVel = ivel.x;
    yVel = ivel.y;

    impact->xVel = evel.x;
    impact->yVel = evel.y;

//    impact->xWalkVel = ewvel.x;
//    impact->yWalkVel = ewvel.y;

    collided = true;
    impact->collidable=false;
}

void dot::updatePos(){
    xVel+=xAcc;
    yVel+=yAcc;
    xPos+=xVel;
    yPos+=yVel;
}

void dot::drawDot(){
    SDL_Rect drawPos;
    drawPos.x=static_cast<int> (xPos - 9.5);
    drawPos.y=static_cast<int> (yPos - 9.5);

    SDL_BlitSurface(sprite, 0, screen, &drawPos);
}
