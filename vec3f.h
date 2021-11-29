#ifndef __VEC3F_H__
#define __VEC3F_H__
#include <iostream>
#include <cmath>

using namespace std;

class Vec3f
{
    public:
        double x, y, z;
        
        Vec3f(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

        Vec3f operator+(Vec3f obj) const;
        Vec3f operator-(Vec3f obj) const;
        Vec3f operator-(void) const;
        Vec3f operator*(Vec3f obj) const;                     // cross product
        Vec3f operator*(double c) const;                      // scalar multiplication

        friend ostream& operator<<(ostream& os, const Vec3f& vec);

        //Vec3f scalarMultiplication(double c);
        double dot(Vec3f obj) const;
        Vec3f normalize() const;                            // get unit vector
        Vec3f elementwiseMultiplication(Vec3f obj) const;           //element-wise matrix multiplication
        double length() const;                                // get vector's length

        Vec3f clamp();
};




#endif

