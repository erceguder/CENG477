#include "ray.h"

Ray::Ray(): origin(0, 0, 0), direction(0, 0, 0) {}

Ray::Ray(Vec3f origin, Vec3f direction): origin(origin), direction(direction){}

Vec3f const Ray::getOrigin(){ return this->origin; }

Vec3f const Ray::getDirection(){ return this->direction; }

void Ray::setOrigin(Vec3f origin) {this->origin = origin; }

void Ray::setDirection(Vec3f direction) { this->direction = direction; }