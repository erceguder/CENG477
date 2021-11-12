#ifndef __VEC3F_H__
#define __VEC3F_H__

class Vec3f
{
    float x, y, z;

    public:
        
        Vec3f(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

        float getX();
        float getY();
        float getZ();

        void setX(float cx);
        void setY(float cy);
        void setZ(float cz);

        Vec3f operator+(Vec3f &obj);
        Vec3f operator-(Vec3f &obj);
        Vec3f operator*(Vec3f &obj);    // cross product

        Vec3f scalarMultiplication(float c);
        float dotProduct(Vec3f obj);        
};




#endif

