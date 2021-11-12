#include "ray.h"

Ray::Ray(){
    Vec3f origin(0,0,0);
    Vec3f direction(0,0,0);
    this->origin = origin;
    this->direction = direction; 
}

Ray::Ray(Vec3f origin, Vec3f direction){
    this->origin = origin;
    this->direction = direction;
}

Vec3f Ray::getOrigin(){ return this->origin; }

Vec3f Ray::getDirection(){ return this->direction; }

void Ray::setOrigin(Vec3f origin) {this->origin = origin; }

void Ray::setDirection(Vec3f direction) { this->direction = direction; }