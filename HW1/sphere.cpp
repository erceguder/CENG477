#include "sphere.h"
#include "parser.h"

using namespace parser;

extern Scene scene;

bool Sphere::intersects(const Ray& ray, double& min_t, Vec3f& normal) const{
    /*
        Returns true if the sphere is the closest object to the camera.
    */
    Vec3f center = scene.vertex_data[this->center_vertex_id - 1];
    Vec3f ray_direction = ray.getDirection();

    Vec3f o_minus_c = ray.getOrigin() - center;                               // o - c
    double d_dot_o_minus_c = ray_direction.dot(o_minus_c);               // d . (o - c)
    double d_dot_d = ray_direction.dot(ray_direction);              // d . d

    double sqrt_discr = sqrt( d_dot_o_minus_c*d_dot_o_minus_c - d_dot_d * (o_minus_c.dot(o_minus_c) - this->radius*this->radius));

    if (!isnan(sqrt_discr)){  // ray and sphere intersect
        double t_1 = (-d_dot_o_minus_c - sqrt_discr) / d_dot_d;
        double t_2 = (-d_dot_o_minus_c + sqrt_discr) / d_dot_d;
        double t = MIN(t_1, t_2);                // does it apply all the time??

        if (t < min_t && t > 0){
            min_t = t;
            normal = (ray.getPoint(t) - center).normalize();
            
            return true;
        }
    }
    return false;
}