#include "face.h"
#include "parser.h"

using namespace parser;

extern Scene scene;

void Face::computeNormal(){
    Vec3f v0 = scene.vertex_data[this->v0_id-1];
    Vec3f v1 = scene.vertex_data[this->v1_id-1];
    Vec3f v2 = scene.vertex_data[this->v2_id-1];

    this->normal = ((v2-v1) * (v0-v1)).normalize();
}