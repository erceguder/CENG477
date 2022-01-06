#include "Scaling.h"


using namespace std;


Scaling::Scaling() {}


Scaling::Scaling(int scalingId, double sx, double sy, double sz)
{
    this->scalingId = scalingId;
    this->sx = sx;
    this->sy = sy;
    this->sz = sz;
}


Matrix4 Scaling::getMatrix() {
    Matrix4 matrix;
    matrix.identity();

    matrix.val[0][0] = this->sx;
    matrix.val[1][1] = this->sy;
    matrix.val[2][2] = this->sz;

    return matrix;
}


ostream &operator<<(ostream &os, const Scaling &s)
{
    os << fixed << setprecision(3) << "Scaling " << s.scalingId << " => [" << s.sx << ", " << s.sy << ", " << s.sz << "]";

    return os;
}
