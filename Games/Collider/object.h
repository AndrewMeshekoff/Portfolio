#ifndef OBJECT_H
#define OBJECT_H

#include "explode.h"

class object{
    protected:
    double mass;

    double xPos;
    double yPos;
    double xVel;
    double yVel;
    double xAcc;
    double yAcc;

    public:
    bool collidable;

    object(double m);
    virtual void updatePos();
    virtual void force(explode*);
    double distance(object*);
    double distance(double, double);
    friend void airRes(object*);
};

#endif
