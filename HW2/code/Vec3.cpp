#include "Vec3.h"


using namespace std;


Vec3::Vec3()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->colorId = -1;
}


Vec3::Vec3(double x, double y, double z, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->colorId = colorId;
}


Vec3::Vec3(const Vec3 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->colorId = other.colorId;
}


double Vec3::get(int index)
{
    switch (index)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
        return this->z;

    default:
        return this->z;
    }
}


Vec3 Vec3::operator+(Vec3 obj) const{
    Vec3 res;
    res.x = this->x + obj.x;
    res.y = this->y + obj.y;
    res.z = this->z + obj.z;
    return res;
}


Vec3 Vec3::operator-(Vec3 obj) const{
    Vec3 res;
    res.x = this->x - obj.x;
    res.y = this->y - obj.y;
    res.z = this->z - obj.z;
    return res;
}


Vec3 Vec3::operator-(void) const{
    return Vec3(-(this->x), -(this->y), -(this->z), this->colorId);
}


Vec3 Vec3::operator*(Vec3 obj) const{
    Vec3 res;
    res.x = this->y * obj.z - this->z * obj.y;
    res.y = this->z * obj.x - this->x * obj.z;
    res.z = this->x * obj.y - this->y * obj.x;
    return res;
}


Vec3 Vec3::operator*(double c) const{
    Vec3 res;
    res.x = this->x * c;
    res.y = this->y * c;
    res.z = this->z * c;
    return res;
}


bool Vec3::operator==(Vec3 obj) const{
    
    if ((ABS((this->x - obj.x)) < EPSILON) && (ABS((this->y - obj.y)) < EPSILON) && (ABS((this->z - obj.z)) < EPSILON))
        return true;
    else
        return false;

}


Vec3 Vec3::normalize() const{
    Vec3 res;
    double length = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
    
    if (length == 0) {
        perror("normalize: length is 0"); 
        return res;
    }
    
    res.x = this->x / length;
    res.y = this->y / length;
    res.z = this->z / length;
    return res;
}


double Vec3::dot(Vec3 obj) const{
    return this->x * obj.x + this->y * obj.y + this->z * obj.z;
}


Vec3 Vec3::elementwiseMultiplication(Vec3 obj) const{
    Vec3 res;
    res.x = this->x * obj.x;
    res.y = this->y * obj.y;
    res.z = this->z * obj.z;
    return res;
}


double Vec3::length() const{
    return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}


ostream& operator<<(ostream& os, const Vec3& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << "]";

    return os;
}