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

void Triangle::applyCulling(){
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
        for (int i=0; i < 3; i++)
            this->lines[i].assignPixels(n_x, n_y);

        int min_x = this->lines[0].v0.x;
        int max_x = this->lines[0].v0.x;

        int min_y = this->lines[0].v0.y;
        int max_y = this->lines[0].v0.y;

        for (int i=1; i < 3; i++){
            int x_val = this->lines[i].v0.x;
            int y_val = this->lines[i].v0.y;

            if (min_x > x_val)
                min_x = x_val;

            if (max_x < x_val)
                max_x = x_val;

            if (min_y > y_val)
                min_y = y_val;

            if (max_y < y_val)
                max_y = y_val;
        }

        for (int y=min_y; y <= max_y; y++){
            for (int x=min_x; x <= max_x; x++){
                double alpha, beta, gamma;

                alpha = lines[1].f(x, y) / lines[1].f(lines[0].v0.x, lines[0].v0.y);
                beta = lines[2].f(x, y) / lines[2].f(lines[1].v0.x, lines[1].v0.y);
                gamma = lines[0].f(x, y) / lines[0].f(lines[2].v0.x, lines[2].v0.y);

                if (alpha >= 0 && beta >= 0 && gamma >= 0){
                    Color color = this->vertices[0].color * alpha;
                    color = color + this->vertices[1].color * beta;
                    color = color + this->vertices[2].color * gamma;

                    image[x][y] = color;
                }
            }
        }
    }
    else{
        for (int i=0; i < 3; i++)
            this->lines[i].draw(image, n_x, n_y);
    }
}