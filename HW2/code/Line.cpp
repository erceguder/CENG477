#include "Line.h"



Line::Line() {}


Line::Line(Vec4 v0, Vec4 v1, double w) {
    this->v0 = v0;
    this->v1 = v1;
    this->w = w;
}


ostream& operator<<(ostream& os, const Line& line) {
    
    os << fixed << setprecision(6) << "Endpoint 1: " << line.v0 << ", Endpoint 2: " << line.v1 << " w value: " << line.w << endl;;

    return os;
}
