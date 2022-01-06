#ifndef __MATRIX4_H__
#define __MATRIX4_H__

#include <iostream>
#include "Vec4.h"

using namespace std;

class Matrix4
{
public:
    double val[4][4];

    Matrix4();
    Matrix4(double val[4][4]);
    Matrix4(const Matrix4 &other);

    void identity();

    Matrix4 operator*(Matrix4 obj) const;                    
    Vec4 operator*(Vec4 obj);

    friend ostream &operator<<(ostream &os, const Matrix4 &m);
};

#endif