#include "Rotation.h"


using namespace std;


Rotation::Rotation() {}


Rotation::Rotation(int rotationId, double angle, double x, double y, double z)
{
    this->rotationId = rotationId;
    this->angle = angle;
    this->ux = x;
    this->uy = y;
    this->uz = z;
}


Matrix4 Rotation::getMatrix() {
    Vec3 u(this->ux, this->uy, this->uz, -1);
    Vec3 v;
    double abs_ux, abs_uy, abs_uz;
    double angle_rad = this->angle * PI/180;

    abs_ux = ABS(u.x);
    abs_uy = ABS(u.y);
    abs_uz = ABS(u.z);

    double min = MIN(abs_ux, MIN(abs_uy, abs_uz));

    if (min == abs_ux) {
        v.x = 0;
        v.y = -1 * u.z;
        v.z = u.y;

    } else if (min == abs_uy) {
        v.x = -1 * u.z;
        v.y = 0;
        v.z = u.x;

    } else {
        v.x = -1 * u.y;
        v.y = u.x;
        v.z = 0;

    }

    Vec3 w = u * v;

    v.normalize();
    w.normalize();

    double entries_m = {
        {u.x, u.y, u.z, 0},
        {v.x, v.y, v.z, 0},
        {w.x, w.y, w.z, 0},
        {0, 0, 0, 1}
    };
    double entries_m_inv = {
        {u.x, v.x, w.x, 0},
        {u.y, v.y, w.y, 0}, 
        {u.z, v.z, w.z, 0},
        {0, 0, 0, 1}
    };
    double entries_Rx = {
        {1, 0, 0, 0},
        {0, cos(angle_rad), -sin(angle_rad), 0},
        {0, sin(angle_rad), cos(angle_rad), 0},
        {0, 0, 0, 1}
    };

    Matrix4 M(entries_m);
    Matrix4 M_inv(entries_m_inv);
    Matrix4 Rx(entries_Rx);

    return M_inv * Rx * M;

}


ostream &operator<<(ostream &os, const Rotation &r)
{
    os << fixed << setprecision(3) << "Rotation " << r.rotationId << " => [angle=" << r.angle << ", " << r.ux << ", " << r.uy << ", " << r.uz << "]";

    return os;
}