#ifndef __RAY_H__
#define __RAY_H__

#include "vec3f.h"

class Ray
{
    Vec3f origin, direction;

    public:
        Ray();
        Ray(Vec3f origin, Vec3f direction);

        Vec3f getOrigin() const;
        Vec3f getDirection() const;

        void setOrigin(Vec3f origin);
        void setDirection(Vec3f direction);

        Vec3f getPoint(double distance);

        friend ostream& operator<<(ostream& os, const Ray& ray);     
};



#endif 
