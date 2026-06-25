#include"cuerpo.h"
#include<string>

cuerpo::cuerpo() {
    radio = 0.0;
    masa = 0.0;
    x = 0.0; 
    y = 0.0;
    vx = 0.0; 
    vy = 0.0;
}

cuerpo::cuerpo(double xi, double yi, double mass, double radius){
    x = xi; 
    y = yi;
    vx = 0.0; 
    vy = 0.0;
    masa = mass;
    radio = radius;
}

double cuerpo::getRadius() {
    return radio;
}

cuerpo::~cuerpo() {}