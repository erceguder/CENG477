#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "ray.h"
#define MIN(a,b) ((a) < (b) ? (a) : (b))

class Sphere{
    public:
        int material_id;
        int center_vertex_id;
        double radius;

        bool intersects(const Ray& ray, double& min_t, Vec3f& normal) const;
};

#endif
