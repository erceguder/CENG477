#include "ray.h"

Ray::Ray(){
    Vec3f origin(0,0,0);
    Vec3f direction(0,0,0);
    this->o = origin;
    this->d = direction; 
}

Ray::Ray(Vec3f origin, Vec3f direction){
    this->o = origin;
    this->d = direction;
}

Vec3f Ray::getO(){ return this->o; }

Vec3f Ray::getD(){ return this->d; }

void Ray::setO(Vec3f origin) {this->o = origin; }

void Ray::setD(Vec3f direction) { this->d = direction; }