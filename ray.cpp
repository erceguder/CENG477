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

Vec3f Ray::getOrigin(){ return this->o; }

Vec3f Ray::getDirection(){ return this->d; }

void Ray::setOrigin(Vec3f origin) {this->o = origin; }

void Ray::setDirection(Vec3f direction) { this->d = direction; }