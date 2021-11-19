#ifndef __FACE_H__
#define __FACE_H__

#include "vec3f.h"

class Face{
    public:
        int v0_id;
        int v1_id;
        int v2_id;
        Vec3f normal;

        void computeNormal();
};

#endif
