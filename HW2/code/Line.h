#ifndef __LINE_H__
#define __LINE_H__

#include <iostream>

#include "Vec4.h"

using namespace std;


class Line{
public:
    Vec4 v0, v1;
    double w;           // for perspective divide

    Line();
    Line(Vec4 v0, Vec4 v1, double w);
    
    friend ostream &operator<<(ostream &os, const Line &m);
};


#endif
