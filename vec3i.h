#ifndef __VEC3I_H__
#define __VEC3I_H__

#include <iostream>
using namespace std;

class Vec3i
{
    public:
        int x, y, z;

        Vec3i(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

        Vec3i operator+(Vec3i &obj);
        Vec3i operator-(Vec3i &obj);
        Vec3i operator*(Vec3i &obj);    // cross product

        Vec3i scalarMultiplication(int c);
        int dotProduct(Vec3i obj);        
        
        friend ostream& operator<<(ostream& os, const Vec3i& vec);
};






#endif
