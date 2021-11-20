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

bool Face::intersects(bool bfc, Ray ray, double& min_t, Vec3f& normal){
    Vec3f a = scene.vertex_data[this->v0_id - 1];
    Vec3f b = scene.vertex_data[this->v1_id - 1];
    Vec3f c = scene.vertex_data[this->v2_id - 1];

    if (bfc && (this->normal.dot(ray.getDirection()) > 0)) return false;   // Back-face culling

    if (ray.getDirection().dot(this->normal) == 0) return false;  // floating point precision loss ?

    double t = ((a - ray.getOrigin()).dot(this->normal)) / (ray.getDirection().dot(this->normal));

    Vec3f p = ray.getPoint(t);

    Vec3f v_p = (p - b) * (a - b);
    Vec3f v_c = (c - b) * (a - b);
    if (v_p.dot(v_c) < 0) return false;

    v_p = (p - a) * (c - a);
    v_c = (b - a) * (c - a);
    if (v_p.dot(v_c) < 0) return false;

    v_p = (p - c) * (b - c);
    v_c = (a - c) * (b - c);
    if (v_p.dot(v_c) < 0) return false;

    if (t < min_t && t > 0){
        min_t = t;
        normal = this->normal;
        return true;
    }
    return false;
}