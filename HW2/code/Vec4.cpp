
#include "Vec4.h"


using namespace std;


Vec4::Vec4(){
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->w = 0.0;
    this->colorId = -1;
}

Vec4::Vec4(double x, double y, double z, double w, int colorId){
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
    this->colorId = colorId;
}

Vec4::Vec4(const Vec4 &other){
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->w = other.w;
    this->colorId = other.colorId;
}

Vec4::Vec4(Vec3 v, double w){
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
    this->w = w;
    this->colorId = v.colorId;
}

void Vec4::perspectiveDivide(){
    this->x = this->x / this->w;
    this->y = this->y / this->w;
    this->z = this->z / this->w;

    this->w = 1;
}

double Vec4::get(int index){
    switch (index)
    {
        case 0:
            return this->x;

        case 1:
            return this->y;

        case 2:
            return this->z;

        case 3:
            return this->w;

        default:
            return this->w;
    }
}

Vec4 Vec4::operator*(Vec4 rhs) const{
    Vec4 res;

    res.x = this->y * rhs.z - this->z * rhs.y;
    res.y = this->z * rhs.x - this->x * rhs.z;
    res.z = this->x * rhs.y - this->y * rhs.x;

    res.w = 0;  // res is a vector
    
    return res;
}

Vec4 Vec4::operator*(double c) const{
    Vec4 res;
    res.x = this->x * c;
    res.y = this->y * c;
    res.z = this->z * c;
    
    res.w = 1;  // res is a vertex

    return res;
}

Vec4 Vec4::operator-(Vec4 rhs) const{
    Vec4 res;

    res.x = this->x - rhs.x;
    res.y = this->y - rhs.y;
    res.z = this->z - rhs.z;

    res.w = 0;  // res is a vector

    return res;
}

double Vec4::dot(Vec4& rhs){
    double res = 0;

    res += this->x * rhs.x;
    res += this->y * rhs.y;
    res += this->z * rhs.z;

    return res;
}

ostream& operator<<(ostream& os, const Vec4& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";

    return os;
}