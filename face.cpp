#include "face.h"
#include "parser.h"

using namespace parser;

extern Scene scene;
extern Camera camera;

void Face::computeNormal(){
    this->v0 = scene.vertex_data[this->v0_id-1];
    this->v1 = scene.vertex_data[this->v1_id-1];
    this->v2 = scene.vertex_data[this->v2_id-1];

    this->normal = ((v2-v1) * (v0-v1)).normalize();
}

// bool Face::intersects(bool bfc, const Ray& ray, double& min_t, Vec3f& normal) const{
//     Vec3f ray_direction = ray.getDirection();
//     double normal_dot_ray_direction = this->normal.dot(ray_direction);

//     if (bfc && (normal_dot_ray_direction > 0)) return false;   // Back-face culling

//     if (normal_dot_ray_direction == 0) return false;  // floating point precision loss ?

//     double t = ((v0 - ray.getOrigin()).dot(this->normal)) / (normal_dot_ray_direction);

//     Vec3f p = ray.getPoint(t);

//     Vec3f v_p = (p - v1) * (v0 - v1);
//     Vec3f v_c = (v2 - v1) * (v0 - v1);
//     if (v_p.dot(v_c) < 0) return false;

//     v_p = (p - v0) * (v2 - v0);
//     v_c = (v1 - v0) * (v2 - v0);
//     if (v_p.dot(v_c) < 0) return false;

//     v_p = (p - v2) * (v1 - v2);
//     v_c = (v0 - v2) * (v1 - v2);
//     if (v_p.dot(v_c) < 0) return false;

//     if (t < min_t && t > 0){
//         min_t = t;
//         normal = this->normal;
//         return true;
//     }
//     return false;
// }

double determinant(const Vec3f& v1, const Vec3f& v2, const Vec3f& v3){
    return v1.x * (v2.y * v3.z - v2.z * v3.y) - v2.x * (v1.y * v3.z - v1.z * v3.y) + v3.x * (v1.y * v2.z - v1.z * v2.y);
}

//bool intersects(Ray ray, Triangle triangle, double* min_distance, Vec3f* normal_p){
bool Face::intersects(bool bfc, const Ray& ray, double& min_t, Vec3f& normal) const{
    //Returns true if the triangle is the closest object to the camera.

    Vec3f direction = ray.getDirection();
    Vec3f origin = ray.getOrigin();

    Vec3f v0_minus_v1 = v0-v1;
    Vec3f v0_minus_v2 = v0-v2;
    Vec3f v0_minus_origin = v0-origin;

    double det_A = determinant(v0_minus_v1, v0_minus_v2, direction);
    double t = determinant(v0_minus_v1, v0_minus_v2, v0_minus_origin) / det_A;

    if (t <= 0)
        return false;
    
    double beta = determinant(v0_minus_origin, v0_minus_v2, direction) / det_A;
    double gamma = determinant(v0_minus_v1, v0_minus_origin, direction) / det_A;

    if (beta >= 0 && gamma >= 0 && (beta + gamma) <= 1)
        if (t < min_t){
            min_t = t;
            normal = this->normal;
            return true;
        }
    return false;
}