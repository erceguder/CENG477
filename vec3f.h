#ifndef __VEC3F_H__
#define __VEC3F_H__
#include <iostream>
#include <cmath>

using namespace std;

class Vec3f
{
    public:
        float x, y, z;
        
        Vec3f(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

        Vec3f operator+(Vec3f obj);
        Vec3f operator-(Vec3f obj);
        Vec3f operator-(void);
        Vec3f operator*(Vec3f obj);    // cross product
        Vec3f operator*(float c);      // scalar multiplication

        friend ostream& operator<<(ostream& os, const Vec3f& vec);

        //Vec3f scalarMultiplication(float c);
        float dot(Vec3f obj);
        Vec3f normalize();                   // get unit vector
        Vec3f elementwiseMultiplication(Vec3f obj);     //element-wise matrix multiplication
        float length();                         // get vector's length
};




#endif

