#include "ray.h"

Ray::Ray(): origin(0, 0, 0), direction(0, 0, 0) {}

Ray::Ray(Vec3f origin, Vec3f direction): origin(origin), direction(direction){}

Vec3f Ray::getOrigin() const { return this->origin; }

Vec3f Ray::getDirection() const { return this->direction; }

void Ray::setOrigin(Vec3f origin) {this->origin = origin; }

void Ray::setDirection(Vec3f direction) { this->direction = direction; }

ostream& operator<<(ostream& os, const Ray& ray){
    os << "origin: " << ray.origin << ", direction: " << ray.direction;
    return os;
}