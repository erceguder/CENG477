#include "vec3f.h"

Vec3f Vec3f::operator+(Vec3f obj){
    Vec3f res;
    res.x = this->x + obj.x;
    res.y = this->y + obj.y;
    res.z = this->z + obj.z;
    return res;
}

Vec3f Vec3f::operator-(Vec3f obj){
    Vec3f res;
    res.x = this->x - obj.x;
    res.y = this->y - obj.y;
    res.z = this->z - obj.z;
    return res;
}

Vec3f Vec3f::operator-(void){
    return Vec3f(-(this->x), -(this->y), -(this->z));
}

Vec3f Vec3f::operator*(Vec3f obj){
    Vec3f res;
    res.x = this->y * obj.z - this->z * obj.y;
    res.y = this->z * obj.x - this->x * obj.z;
    res.z = this->x * obj.y - this->y * obj.x;
    return res;
}

Vec3f Vec3f::operator*(float c){
    Vec3f res;
    res.x = this->x * c;
    res.y = this->y * c;
    res.z = this->z * c;
    return res;
}

Vec3f Vec3f::normalize(){
    Vec3f res;
    float length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    
    // LENGTH == 0 ???
    
    res.x = this->x / length;
    res.y = this->y / length;
    res.z = this->z / length;
    return res;
}

float Vec3f::dot(Vec3f obj){
    return this->x * obj.x + this->y * obj.y + this->z * obj.z;
}

Vec3f Vec3f::elementwiseMultiplication(Vec3f obj){
    Vec3f res;
    res.x = this->x * obj.x;
    res.y = this->y * obj.y;
    res.z = this->z * obj.z;
    return res;
}

float Vec3f::length(){
    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

ostream& operator<<(ostream& os, const Vec3f& vec){
    os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}