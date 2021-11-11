#ifndef __vec3i_h__
#define __vec3i_h__

class Vec3i
{

    int x, y, z;

public:

    Vec3i(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}

    int getX();
    int getY();
    int getZ();

    void setX(int cx);
    void setY(int cy);
    void setZ(int cz);

    Vec3i operator+(Vec3i &obj);
    Vec3i operator-(Vec3i &obj);
    Vec3i operator*(Vec3i &obj);    // cross product

    Vec3i scalarMultiplication(int c);
    int dotProduct(Vec3i obj);        
    
    
};






#endif
