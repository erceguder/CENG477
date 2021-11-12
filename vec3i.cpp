#include "vec3i.h"

int const Vec3i::getX(){ return this->x; }

int const Vec3i::getY(){ return this->y; }

int const Vec3i::getZ(){ return this->z; }

void Vec3i::setX(int cx) { this->x = cx; }

void Vec3i::setY(int cy) { this->y = cy; }

void Vec3i::setZ(int cz) { this->z = cz; }

Vec3i Vec3i::operator+(Vec3i &obj){
    Vec3i res;
    res.x = this->getX() + obj.getX();
    res.y = this->getY() + obj.getY();
    res.z = this->getZ() + obj.getZ();
    return res;
}

Vec3i Vec3i::operator-(Vec3i &obj){
    Vec3i res;
    res.x = this->getX() - obj.getX();
    res.y = this->getY() - obj.getY();
    res.z = this->getZ() - obj.getZ();
    return res;
}

Vec3i Vec3i::operator*(Vec3i &obj){
    Vec3i res;
    res.x = this->getY() * obj.getZ() - this->getZ() * obj.getY();
    res.y = this->getZ() * obj.getX() - this->getX() * obj.getZ();
    res.z = this->getX() * obj.getY() - this->getY() * obj.getX();
    return res;
}

Vec3i Vec3i::scalarMultiplication(int c){
    Vec3i res;
    res.x = this->getX() * c;
    res.y = this->getY() * c;
    res.z = this->getZ() * c;
    return res;
}

int Vec3i::dotProduct(Vec3i obj){
    return this->getX() * obj.getX() + this->getY() * obj.getY() + this->getZ() * obj.getZ();
}

ostream& operator<<(ostream& os, const Vec3i& vec){
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}