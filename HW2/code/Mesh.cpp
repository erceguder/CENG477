#include "Mesh.h"


using namespace std;

Mesh::Mesh() {}

Mesh::Mesh(int meshId, int type, int transformation_count,
             vector<int> transformationIds,
             vector<char> transformationTypes,
             int numberOfTriangles,
             vector<Triangle> triangles)
{
    this->meshId = meshId;
    this->type = type;
    this->transformation_count = transformation_count;
    this->triangle_count = numberOfTriangles;

    this->transformationIds = transformationIds;
    this->transformationTypes = transformationTypes;
    this->triangles = triangles;
}

ostream &operator<<(ostream &os, const Mesh &m)
{
    os << "Mesh " << m.meshId;

    if (m.type == 0)
    {
        os << " wireframe(0) with ";
    }
    else
    {
        os << " solid(1) with ";
    }

    os << fixed << setprecision(3) << m.transformation_count << " transformations and " << m.triangle_count << " triangles"
       << endl << "\tTriangles are:" << endl << fixed << setprecision(0);

    for (int i = 0; i < m.triangles.size(); i++) {
        os << "\t\t" << m.triangles[i].vertexIds[0] << " " << m.triangles[i].vertexIds[1] << " " << m.triangles[i].vertexIds[2] << endl;
    }

    return os;
}
