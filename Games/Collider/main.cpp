#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

#include "timer.h"
#include "vec2D.h"
#include "explode.h"
#include "barrier.h"
#include "object.h"
#include "dot.h"
#include "player.h"

using namespace std;

SDL_Surface* screen = NULL;

SDL_Surface *Load_image (string filename){

    SDL_Surface* loaded_img = 0;
    SDL_Surface* optimized_img = 0;

    loaded_img = IMG_Load(filename.c_str());
    optimized_img = SDL_DisplayFormat(loaded_img);

    SDL_FreeSurface(loaded_img);

    return optimized_img;
}

void color_key (SDL_Surface* image, int R, int G, int B){
    Uint32 color = SDL_MapRGB(image->format, R, G, B);
    SDL_SetColorKey(image, SDL_SRCCOLORKEY, color);
}

void airRes(object* item){
//    if(item->collidable){
        if(sqrt(pow(item->xVel, 2)+pow(item->yVel, 2))<0.4){
            item->yVel=0;
            item->xVel=0;
            item->yAcc=0;
            item->xAcc=0;
        }
        else{
            if(abs(item->xVel)>0.4)
                item->xAcc=-0.08*(item->xVel);
            if(abs(item->yVel)>0.4)
                item->yAcc=-0.08*(item->yVel);
        }
//    }
}

int main(int argc, char* argv[]){

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Event action;
    timer limit;
    int time = 0;

    screen = SDL_SetVideoMode(600, 600, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption("Yoshi Collisions!", NULL);

    player * move = new player("yoshi2.bmp");

    explode* test = new explode(300, 300, 6, 0);
    dot* group[6];
    group[0] = new dot(275, 300);
    group[1] = new dot(280, 280);
    group[2] = new dot(320, 280);
    group[3] = new dot(325, 300);
    group[4] = new dot(280, 320);
    group[5] = new dot(320, 320);

    vector<barrier*> walls(6);
    walls.at(0) = new barrier(0, -20, 620, 20, 0);
    walls.at(1) = new barrier(600, 0, 20, 600, 0);
    walls.at(2) = new barrier(-20, 0, 20, 600, 0);
    walls.at(3) = new barrier(0, 600, 600, 20, 0);
    walls.at(4) = new barrier(100, 500, 400, 20, 0);


    timer countdown;
    countdown.start();

    Uint32 color = SDL_MapRGB(screen->format, 0xBF, 0xFF, 0xBF);

    SDL_FillRect(screen, &(screen->clip_rect), color);
    SDL_Flip(screen);

    while(action.type!=SDL_QUIT){

        limit.start();

        //Get Input
        while(SDL_PollEvent(&action)){
            if((action.type==SDL_KEYDOWN)||(action.type==SDL_KEYUP)){
                move->input(action);
            }
        }
        if(countdown.getTime()>5000){
            for(unsigned i=0; i<6; i++){
                group[i]->force(test);
            }
            move->force(test);
            countdown.stop();
//            countdown.start();
        }

        for(unsigned i=0; i<6; i++){
            group[i]->updatePos();
        }
        move->updatePos();

//        for(unsigned i=0; i<6; i++){
//            airRes(group[i]);
//        }

        airRes(move);

        move->collided=false;
        for(unsigned i=0; i<6; i++){
            group[i]->collide(move);
            for(unsigned j=0; j<6; j++){
                if((i!=j)&&(!group[j]->collided))
                    group[i]->collide(group[j]);
            }
            group[i]->collided = true;
        }
        for(unsigned i=0; i<6; i++){
            group[i]->collided = false;
        }
        if(!move->collided)
            move->collidable=true;

        for(unsigned i=0; i<5; i++){
            if(move->checkCollision(walls.at(i))){
                    move->bounce(move->getNormal(walls.at(i)));
                    move->block(move->getNormal(walls.at(i)));
            }
            for(unsigned j=0; j<6; j++){
                group[j]->collide(walls.at(i));
            }
        }


        //Draw
        SDL_FillRect(screen, &screen->clip_rect, color);
        SDL_FillRect(screen, &(walls[4]->draw_box), SDL_MapRGB(screen->format, 0, 200, 0));
        for(unsigned i=0; i<6; i++){
            group[i]->drawDot();
        }
        move->drawPlayer();

        time = limit.getTime();
        limit.stop();
        if(time<(1000.0/60)){
            SDL_Delay(1000/60-time);
        }

        SDL_Flip(screen);
    }

    SDL_FreeSurface(screen);
    SDL_Quit();
    delete move;
    for(unsigned i=0; i<6; i++){
        delete group[i];
    }

return 0;
}
