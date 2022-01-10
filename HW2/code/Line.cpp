#include "Line.h"

Line::Line(Vec4 v0, Vec4 v1) {
    this->v0 = v0;
    this->v1 = v1;
    this->rejected = false;
}

bool Line::visible(double den, double num, double &t_e, double &t_l){
    double t = num/den;

    if (den > 0) {        // pot. entering
        if (t > t_l) return false;
        if (t > t_e) t_e = t;

    } else if (den < 0) {     // pot. leaving
        if (t < t_e) return false;
        if (t < t_l) t_l = t;

    } else if (num > 0) 
        return false;

    return true;
}

void Line::clip(double x_min=-1, double x_max=1, double y_min=-1, double y_max=1, double z_min=-1, double z_max=1){
    double d_x, d_y, d_z;
    double t_e = 0;
    double t_l = 1;
    
    d_x = v1.x - v0.x;
    d_y = v1.y - v0.y;
    d_z = v1.z - v0.z;

    if (d_x == 0)
        if ((x_min > this->v0.x) || (this->v0.x > x_max))
            this->rejected = true;

    if (d_y == 0)
        if ((y_min > this->v0.y) || (this->v0.y > y_max))
            this->rejected = true;

    if (d_z == 0)
        if ((z_min > this->v0.z) || (this->v0.z > z_max))
            this->rejected = true;

    if (this->rejected)
        return;

    Color diff = v1.color - v0.color;

    if (visible(d_x, x_min - this->v0.x, t_e, t_l) && visible(-d_x, this->v0.x - x_max, t_e, t_l) &&
        visible(d_y, y_min - this->v0.y, t_e, t_l) && visible(-d_y, this->v0.y - y_max, t_e, t_l) && 
        visible(d_z, z_min - this->v0.z, t_e, t_l) && visible(-d_z, this->v0.z - z_max, t_e, t_l)){

        if (t_l < 1){
            v1.x = v0.x + d_x * t_l;
            v1.y = v0.y + d_y * t_l;
            v1.z = v0.z + d_z * t_l;

            v1.color = v0.color + (diff * t_l);
        }
        if (t_e > 0){
            v0.x = v0.x + d_x * t_e;
            v0.y = v0.y + d_y * t_e;
            v0.z = v0.z + d_z * t_e;
        }
    }
}

ostream& operator<<(ostream& os, const Line& line) {
    
    os << fixed << setprecision(6) << "Endpoint 1: " << line.v0 << ", Endpoint 2: " << line.v1 << endl;;

    return os;
}
