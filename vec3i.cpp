#include "vec3i.h"

Vec3i Vec3i::operator+(Vec3i &obj){
    Vec3i res;
    res.x = this->x + obj.x;
    res.y = this->y + obj.y;
    res.z = this->z + obj.z;
    return res;
}

Vec3i Vec3i::operator-(Vec3i &obj){
    Vec3i res;
    res.x = this->x - obj.x;
    res.y = this->y - obj.y;
    res.z = this->z - obj.z;
    return res;
}

Vec3i Vec3i::operator*(Vec3i &obj){
    Vec3i res;
    res.x = this->y * obj.z - this->z * obj.y;
    res.y = this->z * obj.x - this->x * obj.z;
    res.z = this->x * obj.y - this->y * obj.x;
    return res;
}

Vec3i Vec3i::scalarMultiplication(int c){
    Vec3i res;
    res.x = this->x * c;
    res.y = this->y * c;
    res.z = this->z * c;
    return res;
}

int Vec3i::dotProduct(Vec3i obj){
    return this->x * obj.x + this->y * obj.y + this->z * obj.z;
}

ostream& operator<<(ostream& os, const Vec3i& vec){
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}