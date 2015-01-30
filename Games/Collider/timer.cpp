#include "SDL/SDL.h"
#include "timer.h"

timer::timer(){
    started = false;
    paused = false;
    startingPos = 0;
    pausePos = 0;
}

void timer::start(){
    started = true;
    startingPos = SDL_GetTicks();
}

void timer::pause(){
    if(started){
        paused = true;
        pausePos = SDL_GetTicks();
    }
}

void timer::stop(){
    started = false;
    paused = false;
    startingPos = 0;
    pausePos = 0;
}

void timer::unpause(){
    paused = false;
    startingPos = SDL_GetTicks()-(pausePos-startingPos);
}

int timer::getTime(){
    if(started){
        if(paused){
            return (pausePos-startingPos);
        }
        else{
            return (SDL_GetTicks()-startingPos);
        }
    }
    return 0;
}

bool timer::isPaused(){
    return paused;
}

