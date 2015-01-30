#ifndef VEC2D_H
#define VEC2D_H

struct vec2D{
    double x, y;
    double dotProd(vec2D);
    vec2D operator-(const vec2D&);
    vec2D operator+(const vec2D&);
    vec2D scalProd(double);
};

#endif
