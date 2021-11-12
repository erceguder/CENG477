#ifndef __VEC3F_H__
#define __VEC3F_H__
#include <iostream>

using namespace std;

class Vec3f
{
    float x, y, z;

    public:
        
        Vec3f(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

        float const getX();
        float const getY();
        float const getZ();

        void setX(float cx);
        void setY(float cy);
        void setZ(float cz);

        Vec3f operator+(Vec3f &obj);
        Vec3f operator-(Vec3f &obj);
        Vec3f operator*(Vec3f &obj);    // cross product

        Vec3f scalarMultiplication(float c);
        float dotProduct(Vec3f obj);

        friend ostream& operator<<(ostream& os, const Vec3f& vec);     
};




#endif

