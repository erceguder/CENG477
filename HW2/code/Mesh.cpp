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


void Mesh::fillLinesVector(){

    int vertice_count = this->vertices.size();
    for (int i=0; i<vertice_count; i+=3){

        // ccw order
        Line l1(this->vertices[i], this->vertices[i+1]);
        Line l2(this->vertices[i+1], this->vertices[i+2]);
        Line l3(this->vertices[i+2], this->vertices[i]);

        this->lines.push_back(l1);
        this->lines.push_back(l2);
        this->lines.push_back(l3);

    }

}


void Mesh::emptyLinesVector(){
    this->lines.clear();
}


void Mesh::fillVerticesVector(vector<Vec3*> *vertices){

    if (this->vertices.empty())
        for (int i=0; i<this->triangle_count; i++)
            for (int j=0; j<3; j++){

                Vec4 v(*((*vertices)[this->triangles[i].vertexIds[j]-1]), 1);
                this->vertices.push_back(v);
            
            }

}


void Mesh::emptyVerticesVector(){
    this->vertices.clear();
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
