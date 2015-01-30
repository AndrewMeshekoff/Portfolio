#include <cstdlib>
#include <string>
#include <cmath>
#include "SDL/SDL.h"
#include "player.h"
#include "barrier.h"
#include "vec2D.h"

extern SDL_Surface* screen;

SDL_Surface* Load_image(std::string);
void color_key(SDL_Surface* image, int R, int G, int B);

player::player(std::string filename): object(1.0){
    xPos = 40;
    yPos = 40;
    xWalkAcc = 0;
    yWalkAcc = 0;
    xWalkVel = 0;
    yWalkVel = 0;
    xNetVel = 0;
    yNetVel = 0;

    keyState[0] = false;
    keyState[1] = false;
    keyState[2] = false;
    keyState[3] = false;
    xCurr = 0;
    yCurr = 0;
    walkDist = 0;
    frame = 0;
    Frame.h = 64;
    Frame.w = 64;
    Frame.x = 0;
    h = 64;
    w = 30;

    sprite = Load_image(filename);
    color_key(sprite, 255, 0, 255);
}

player::~player(){
    SDL_FreeSurface(sprite);
}

void player::input(SDL_Event action){
    if(action.type==SDL_KEYDOWN){
        if(action.key.keysym.sym==SDLK_w){
            keyState[0] = true;
            yCurr = -1;
            yWalkAcc=-0.2;
        }
        else if(action.key.keysym.sym==SDLK_s){
            keyState[1] = true;
            yCurr = 1;
            yWalkAcc=0.2;
        }
        else if(action.key.keysym.sym==SDLK_d){
            keyState[2] = true;
            xCurr = 1;
            xWalkAcc=0.2;
        }
        else if(action.key.keysym.sym==SDLK_a){
            keyState[3] = true;
            xCurr = -1;
            xWalkAcc=-0.2;
        }
    }
    else if(action.type==SDL_KEYUP){
        if(action.key.keysym.sym==SDLK_w){
            if(yCurr<0){
                if(keyState[1]){
                    yCurr = 1;
                    yWalkAcc=0.2;
                }
                else{
                    yCurr = 0;
                    yWalkAcc=0;
                }
            }
            keyState[0] = false;
        }
        else if(action.key.keysym.sym==SDLK_s){
            if(yCurr>0){
                if(keyState[0]){
                    yCurr = -1;
                    yWalkAcc=-0.2;
                }
                else{
                    yCurr = 0;
                    yWalkAcc=0;
                }
            }
            keyState[1] = false;
        }
        else if(action.key.keysym.sym==SDLK_d){
            if(xCurr>0){
                if(keyState[3]){
                    xCurr = -1;
                    xWalkAcc=-0.2;
                }
                else{
                    xCurr = 0;
                    xWalkAcc=0;
                }
            }
            keyState[2] = false;
        }
        else if(action.key.keysym.sym==SDLK_a){
            if(xCurr<0){
                if(keyState[2]){
                    xCurr = 1;
                    xWalkAcc=0.2;
                }
                else{
                    xCurr = 0;
                    xWalkAcc=0;
                }
            }
            keyState[3] = false;
        }
    }
}

void player::walkDecay(){
    if(!(keyState[2]||keyState[3])&&(xWalkVel!=0)){
        if(std::abs(xWalkVel)<=0.2)
            xWalkVel=0;
        else if(xWalkVel<0)
            xWalkVel+=0.2;
        else
            xWalkVel-=0.2;
    }
    if(!(keyState[0]||keyState[1])&&(yWalkVel!=0)){
        if(std::abs(yWalkVel)<=0.2)
            yWalkVel=0;
        else if(yWalkVel<0)
            yWalkVel+=0.2;
        else
            yWalkVel-=0.2;
    }
}

void player::updateFrame(){
    if(walkDist>=8){
        walkDist = 0;
        frame++;
        if(frame>=8)
            frame=0;
    }
    if(xCurr>0)
        Frame.x = 0;
    else if(xCurr<0)
        Frame.x = 64;
    if((std::abs(xWalkVel)+std::abs(yWalkVel))==0){
        frame = 0;
        walkDist = 0;
    }
}

void player::updatePos(){
    walkDecay();
    xVel+=xAcc;
    yVel+=yAcc;
    bool sameXDir = false;
    bool sameYDir = false;
    if(xCurr<0&&xWalkVel<0)
        sameXDir = true;
    else if(xCurr>0&&xWalkVel>0)
        sameXDir = true;

    if(yCurr<0&&yWalkVel<0)
        sameYDir = true;
    else if(yCurr>0&&yWalkVel>0)
        sameYDir = true;

    if((std::abs(xWalkVel + xWalkAcc)<=4)&&(yCurr==0&&xCurr!=0))
        xWalkVel+=xWalkAcc;
    else if (yCurr!=0&&xCurr!=0){
        if(std::abs(xWalkVel + xWalkAcc)<=2.8||(!sameXDir))
            xWalkVel+=xWalkAcc;
        else if(std::abs(xWalkVel + xWalkAcc)>2.8)
            xWalkVel-=xWalkAcc;
    }
    else
        xWalkVel*=0.9;

    if((std::abs(yWalkVel + yWalkAcc)<=4)&&(xCurr==0&&yCurr!=0))
        yWalkVel+=yWalkAcc;
    else if (yCurr!=0&&xCurr!=0){
        if(std::abs(yWalkVel + yWalkAcc)<=2.8||(!sameYDir))
            yWalkVel+=yWalkAcc;
        else if(std::abs(yWalkVel + yWalkAcc)>2.8)
            yWalkVel-=yWalkAcc;
    }
    else
        yWalkVel*=0.9;

    xPos+=xVel+xWalkVel;
    yPos+=yVel+yWalkVel;

    if((keyState[0]||keyState[1])&&(keyState[2]||keyState[3]))
        walkDist+=sqrt(pow(xWalkVel,2)+pow(yWalkVel,2));
    else
        walkDist+=(std::abs(xWalkVel)+std::abs(yWalkVel));
    updateFrame();
}

bool player::checkCollision(barrier* block){
    if (((xPos+w/2.0)>=block->xPos)&&((xPos-w/2.0)<=(block->xPos+block->w))&&((yPos+h/2.0)>=block->yPos)&&((yPos-h/2.0)<=(block->yPos+block->h))){
        return true;
    }
    return false;
}

vec2D player::getNormal(barrier* block){
    vec2D norm;
    norm.x = 0.0;
    norm.y = 0.0;

    double right = std::abs(block->xPos+block->w-(xPos-w/2.0));
    double left = std::abs(xPos+w/2.0-(block->xPos));
    double down = std::abs(block->yPos+block->h-(yPos-h/2.0));
    double up = std::abs(yPos+h/2.0-(block->yPos));

    if((right<left)&&(right<down)&&(right<up))
        norm.x = 1.0;
    else if((left<right)&&(left<down)&&(left<up))
        norm.x = -1.0;
    else if((down<right)&&(down<left)&&(down<up))
        norm.y = 1.0;
    else if((up<right)&&(up<left)&&(up<down))
        norm.y = -1.0;
    return norm;
}

void player::bounce(vec2D norm){
    vec2D vel;
    vel.x=xVel;
    vel.y=yVel;
    vec2D per = norm.scalProd(vel.dotProd(norm)/norm.dotProd(norm));
    vec2D par = vel-per;
    vel = par-per;
    xVel = vel.x;
    yVel = vel.y;
}

void player::block(vec2D norm){
    double length = sqrt(pow(norm.x,2)+pow(norm.y,2));
    norm.x=norm.x/length;
    norm.y=norm.y/length;

    xPos+=std::abs(xWalkVel)*norm.x;
    yPos+=std::abs(yWalkVel)*norm.y;

    xWalkVel+=std::abs(xWalkVel+xWalkAcc)*norm.x;
    yWalkVel+=std::abs(yWalkVel+yWalkAcc)*norm.y;

//    yWalkAcc+=std::abs(yWalkAcc)*norm.y;
//    xWalkAcc+=std::abs(xWalkAcc)*norm.x;
}

void player::drawPlayer(){
    SDL_Rect drawPos;
    drawPos.x=static_cast<int> (xPos - 31.5);
    drawPos.y=static_cast<int> (yPos - 31.5);

    Frame.y = 64*frame;

    SDL_BlitSurface(sprite, &Frame, screen, &drawPos);
}
