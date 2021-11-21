#include "face.h"
#include "parser.h"

using namespace parser;

extern Scene scene;

void Face::computeNormal(){
    this->v0 = scene.vertex_data[this->v0_id-1];
    this->v1 = scene.vertex_data[this->v1_id-1];
    this->v2 = scene.vertex_data[this->v2_id-1];

    this->normal = ((v2-v1) * (v0-v1)).normalize();
}

bool Face::intersects(bool bfc, Ray ray, double& min_t, Vec3f& normal){
    Vec3f ray_direction = ray.getDirection();
    double normal_dot_ray_direction = this->normal.dot(ray_direction);

    if (bfc && (normal_dot_ray_direction > 0)) return false;   // Back-face culling

    if (normal_dot_ray_direction == 0) return false;  // floating point precision loss ?

    double t = ((v0 - ray.getOrigin()).dot(this->normal)) / (normal_dot_ray_direction);

    Vec3f p = ray.getPoint(t);

    Vec3f v_p = (p - v1) * (v0 - v1);
    Vec3f v_c = (v2 - v1) * (v0 - v1);
    if (v_p.dot(v_c) < 0) return false;

    v_p = (p - v0) * (v2 - v0);
    v_c = (v1 - v0) * (v2 - v0);
    if (v_p.dot(v_c) < 0) return false;

    v_p = (p - v2) * (v1 - v2);
    v_c = (v0 - v2) * (v1 - v2);
    if (v_p.dot(v_c) < 0) return false;

    if (t < min_t && t > 0){
        min_t = t;
        normal = this->normal;
        return true;
    }
    return false;
}