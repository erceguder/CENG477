#ifndef __ROTATION_H__
#define __ROTATION_H__

#include <iostream>
#include <iomanip>
#include <cmath>

#include "Matrix4.h"
#include "Vec3.h"


#define ABS(a) ((a) > 0 ? (a) : -1 * (a))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define PI 3.14159265

using namespace std;


class Rotation
{
public:
    int rotationId;
    double angle, ux, uy, uz;

    Rotation();
    Rotation(int rotationId, double angle, double x, double y, double z);

    Matrix4 getMatrix();

    friend ostream &operator<<(ostream &os, const Rotation &r);
};

#endif
