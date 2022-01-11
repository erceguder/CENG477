#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Vec4.h"
#include "Line.h"
#include <vector>

class Triangle
{
public:
    int vertexIds[3];
    Vec4 vertices[3];
    Line lines[3];

    Triangle();
    Triangle(int vid1, int vid2, int vid3, vector<Vec3*>);
    Triangle(const Triangle &other);

    void setLines();

    void clip();
};


#endif
