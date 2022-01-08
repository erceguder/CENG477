#include "Line.h"



Line::Line() {}


Line::Line(Vec4 v0, Vec4 v1) {
    this->v0 = v0;
    this->v1 = v1;
}


bool Line::visible(double d, double dist_to_egde, double &t_e, double &t_l){

    double t = dist_to_egde/d;

    if (d > 0) {        // pot. entering

        if (t > t_l) return false;
        if (t > t_e) t_e = t;

    } else if (d < 0) {     // pot. leaving

        if (t < t_e) return false;
        if (t < t_l) t_l = t;

    } else if (dist_to_egde > 0) 
        return false;

    return true;

}


ostream& operator<<(ostream& os, const Line& line) {
    
    os << fixed << setprecision(6) << "Endpoint 1: " << line.v0 << ", Endpoint 2: " << line.v1 << endl;;

    return os;
}
