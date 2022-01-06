#ifndef __VEC4_H__
#define __VEC4_H__

#include <iostream>
#include <iomanip>

using namespace std;

class Vec4
{
public:
    double x, y, z, t;
    int colorId;


    Vec4();
    Vec4(double x, double y, double z, double t, int colorId);
    Vec4(const Vec4 &other);
    
    double get(int index);

    friend std::ostream& operator<<(std::ostream& os, const Vec4& v);
};

#endif
