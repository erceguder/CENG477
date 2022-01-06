#ifndef __VEC3_H__
#define __VEC3_H__

#include <iostream>
#include <errno.h>
#include <cmath>
#include <iomanip>

#define ABS(a) ((a) > 0 ? (a) : -1 * (a))
#define EPSILON 0.000000001

using namespace std;

class Vec3
{
public:
    double x, y, z;
    int colorId;

    Vec3();
    Vec3(double x, double y, double z, int colorId);
    Vec3(const Vec3 &other);

    double get(int index);

    Vec3 operator+(Vec3 obj) const;
    Vec3 operator-(Vec3 obj) const;
    Vec3 operator-(void) const;
    Vec3 operator*(Vec3 obj) const;                      // cross product
    Vec3 operator*(double c) const;                      // scalar multiplication
    bool operator==(Vec3 obj) const;                     // equality check

    double dot(Vec3 obj) const;
    Vec3 normalize() const;                               // get unit vector
    Vec3 elementwiseMultiplication(Vec3 obj) const;       //element-wise multiplication
    double length() const;                                // get vector's length
    
    friend std::ostream& operator<<(std::ostream& os, const Vec3& v);
};

#endif
