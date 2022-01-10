#ifndef __VEC4_H__
#define __VEC4_H__

#include <iostream>
#include <iomanip>

#include "Vec3.h"
#include "Color.h"

using namespace std;

class Vec4
{
public:
    double x, y, z, w;
    int colorId;
    Color color;

    Vec4();
    Vec4(double x, double y, double z, double w, int colorId);
    Vec4(const Vec4 &other);
    Vec4(Vec3 v, double w);

    Vec4 operator*(double c) const;                  // scalar multiplication
    
    double get(int index);

    friend std::ostream& operator<<(std::ostream& os, const Vec4& v);
};

#endif
