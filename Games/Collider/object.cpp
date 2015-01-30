#include <cstdlib>
#include <cmath>
#include "object.h"

object::object(double m){
    mass = m;

    xPos = 0;
    yPos = 0;
    xVel = 0;
    yVel = 0;
    xAcc = 0;
    yAcc = 0;

    collidable = true;
}

void object::updatePos(){
    xVel+=xAcc;
    yVel+=yAcc;
    xPos+=xVel;
    yPos+=yVel;
}

void object::force(explode* hit){
    double xDist = xPos-(hit->xPos);
    double yDist = yPos-(hit->yPos);
    double dist = sqrt(pow(xDist, 2)+pow(yDist, 2));

    //double impulse = (hit->force)/(pow(dist, 2));
    double impulse = (hit->force)*(60/(dist+60));

    double xPush=impulse*cos(atan(abs(yDist/xDist)))/mass;
    double yPush=impulse*sin(atan(abs(yDist/xDist)))/mass;

    if(xDist<0)
        xPush=-1*xPush;
    if(yDist<0)
        yPush=-1*yPush;
    if(yDist==0&&xDist==0)
        return;

    xVel+=xPush;
    yVel+=yPush;
}

double object::distance(object* point){
    double xDist = xPos - (point->xPos);
    double yDist = yPos - (point->yPos);
    return sqrt(pow(xDist, 2) + pow(yDist, 2));
}

double object::distance(double x, double y){
    double xDist = (xPos - x);
    double yDist = (yPos - y);
    return sqrt(pow(xDist, 2) + pow(yDist, 2));
}
