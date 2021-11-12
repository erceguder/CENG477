#ifndef __RAY_H__
#define __RAY_H__

#include "vec3f.h"

class Ray
{
    Vec3f origin, direction;

    public:
        Ray();
        Ray(Vec3f origin, Vec3f direction);

        Vec3f getOrigin();
        Vec3f getDirection();

        void setOrigin(Vec3f origin);
        void setDirection(Vec3f direction);


};



#endif 
