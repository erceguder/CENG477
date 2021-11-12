#ifndef __RAY_H__
#define __RAY_H__

#include "vec3f.h"

class Ray
{
    Vec3f o, d;

    public:
        Ray();
        Ray(Vec3f origin, Vec3f direction);

        Vec3f getO();
        Vec3f getD();

        void setO(Vec3f origin);
        void setD(Vec3f direction);


};



#endif 
