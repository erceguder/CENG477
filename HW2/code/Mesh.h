#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <iostream>
#include <iomanip>

#include "Triangle.h"
#include "Vec4.h"

using namespace std;

class Mesh
{

public:
    int meshId;
    int type; // 0 for wireframe, 1 for solid
    int transformation_count;
    vector<int> transformationIds;
    vector<char> transformationTypes;
    int triangle_count;
    vector<Triangle> triangles;
    vector<Vec4> vertices;

    Mesh();
    Mesh(int meshId, int type, int transformation_count,
          vector<int> transformationIds,
          vector<char> transformationTypes,
          int triangle_count,
          vector<Triangle> triangles);

    friend ostream &operator<<(ostream &os, const Mesh &m);
};

#endif
