#ifndef __LINE_H__
#define __LINE_H__

#include <iostream>

#include "Vec4.h"

using namespace std;


class Line{
public:
    Vec4 v0, v1;

    Line();
    Line(Vec4 v0, Vec4 v1);

    bool visible(double d, double dist_to_egde, double &t_e, double &t_l);
    
    friend ostream &operator<<(ostream &os, const Line &m);
};


#endif
