
#include "Vec4.h"


using namespace std;


Vec4::Vec4()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->w = 0.0;
    this->colorId = -1;
}


Vec4::Vec4(double x, double y, double z, double w, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
    this->colorId = colorId;
}


Vec4::Vec4(const Vec4 &other)
{
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

    // this->color = *(scene->colorsOfVertices[this->colorId]);
}


double Vec4::get(int index)
{
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


Vec4 Vec4::operator*(double c) const {
    Vec4 res;
    res.x = this->x * c;
    res.y = this->y * c;
    res.z = this->z * c;
    
    // ???????
    res.w = this->w * c;

    return res;
}


ostream& operator<<(ostream& os, const Vec4& v) {
    
    os << fixed << setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";

    return os;
}