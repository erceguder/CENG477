#include "vec3f.h"

float const Vec3f::getX(){ return this->x; }

float const Vec3f::getY(){ return this->y; }

float const Vec3f::getZ(){ return this->z; }

void Vec3f::setX(float cx) { this->x = cx; }

void Vec3f::setY(float cy) { this->y = cy; }

void Vec3f::setZ(float cz) { this->z = cz; }

Vec3f Vec3f::operator+(Vec3f &obj){
    Vec3f res;
    res.x = this->getX() + obj.getX();
    res.y = this->getY() + obj.getY();
    res.z = this->getZ() + obj.getZ();
    return res;
}

Vec3f Vec3f::operator-(Vec3f &obj){
    Vec3f res;
    res.x = this->getX() - obj.getX();
    res.y = this->getY() - obj.getY();
    res.z = this->getZ() - obj.getZ();
    return res;
}

Vec3f Vec3f::operator*(Vec3f &obj){
    Vec3f res;
    res.x = this->getY() * obj.getZ() - this->getZ() * obj.getY();
    res.y = this->getZ() * obj.getX() - this->getX() * obj.getZ();
    res.z = this->getX() * obj.getY() - this->getY() * obj.getX();
    return res;
}

Vec3f Vec3f::scalarMultiplication(float c){
    Vec3f res;
    res.x = this->getX() * c;
    res.y = this->getY() * c;
    res.z = this->getZ() * c;
    return res;
}

float Vec3f::dotProduct(Vec3f obj){
    return this->getX() * obj.getX() + this->getY() * obj.getY() + this->getZ() * obj.getZ();
}

ostream& operator<<(ostream& os, const Vec3f& vec){
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}