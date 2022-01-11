#include "Triangle.h"

Triangle::Triangle(){
    this->vertexIds[0] = -1;
    this->vertexIds[1] = -1;
    this->vertexIds[2] = -1;
}

Triangle::Triangle(int vid1, int vid2, int vid3){
    this->vertexIds[0] = vid1;
    this->vertexIds[1] = vid2;
    this->vertexIds[2] = vid3;

    this->culled = false;
}

Triangle::Triangle(const Triangle &other){
    this->vertexIds[0] = other.vertexIds[0];
    this->vertexIds[1] = other.vertexIds[1];
    this->vertexIds[2] = other.vertexIds[2];
}

void Triangle::setVertices(vector<Vec3*>& vertices){
    for (int i=0; i < 3; i++)
        this->vertices[i] = Vec4(*(vertices[this->vertexIds[i] - 1]), 1);

    this->culled = false;
}

void Triangle::setColours(vector<Color*>& colours){
    for (int i=0; i < 3; i++)
        this->vertices[i].color = *(colours[this->vertices[i].colorId - 1]);
}

void Triangle::setLines(){
    for (int i=0; i < 3; i++)
        this->lines[i] = Line(this->vertices[i], this->vertices[(i+1) % 3]);
}

void Triangle::clip(){
    for (int i=0; i < 3; i++)
        lines[i].clip();
}

void Triangle::applyCulling(Vec4 cam_pos){
    Vec4 eye_vector = this->vertices[0];

    Vec4 normal = (this->vertices[1] - this->vertices[0]) * (this->vertices[2] - this->vertices[0]);

    if (normal.dot(eye_vector) >= 0)
        this->culled = true;
}

void Triangle::draw(vector<vector<Color > >& image, bool solid, int n_x, int n_y){
    if (this->culled)
        return;

    if (solid){
        // Triangle rasterization
    }
    else{
        for (int i=0; i < 3; i++)
            this->lines[i].draw(image, n_x, n_y);
    }
}