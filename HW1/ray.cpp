#include "ray.h"

Ray::Ray(): origin(0, 0, 0), direction(0, 0, 0) {}

Ray::Ray(Vec3f org, Vec3f dir): origin(org), direction(dir){}

Vec3f Ray::getOrigin() const { return this->origin; }

Vec3f Ray::getDirection() const { return this->direction; }

Vec3f Ray::getPoint(double distance) const{
    return origin + direction * distance;
}

ostream& operator<<(ostream& os, const Ray& ray){
    os << "origin: " << ray.origin << ", direction: " << ray.direction;
    return os;
}