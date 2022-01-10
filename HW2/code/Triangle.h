#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Vec4.h"
#include "Line.h"
#include "Mesh.h"

class Triangle
{
public:
    int vertexIds[3];
    Vec4 vertices[3];
    Line lines[3];

    Triangle();
    Triangle(int vid1, int vid2, int vid3);
    Triangle(const Triangle &other);

    void setVariables(Scene* scene);

    void clip();

    // int getFirstVertexId();
    // int getSecondVertexId();
    // int getThirdVertexId();

    // void setFirstVertexId(int vid);
    // void setSecondVertexId(int vid);
    // void setThirdVertexId(int vid);
};


#endif
