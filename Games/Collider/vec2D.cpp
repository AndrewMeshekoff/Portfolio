#include "vec2D.h"

double vec2D::dotProd(vec2D prod){
    return (x*prod.x+y*prod.y);
}

vec2D vec2D::operator-(const vec2D& lhs){
    vec2D res;
    res.x=x-lhs.x;
    res.y=y-lhs.y;
    return res;
}

vec2D vec2D::operator+(const vec2D& lhs){
    vec2D res;
    res.x=x+lhs.x;
    res.y=y+lhs.y;
    return res;
}

vec2D vec2D::scalProd(double mult){
    vec2D res;
    res.x=x*mult;
    res.y=y*mult;
    return res;
}
