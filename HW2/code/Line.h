#ifndef __LINE_H__
#define __LINE_H__

#include <iostream>
#include <float.h>

#include "Vec4.h"
#include <vector>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

using namespace std;

class Line{
public:
    Vec4 v0, v1;
    bool rejected;

    Line();
    Line(Vec4 v0, Vec4 v1);

    bool visible(double den, double num, double &t_e, double &t_l);
    void clip(double x_min=-1, double x_max=1, double y_min=-1, double y_max=1, double z_min=-1, double z_max=1);
    double f(double x, double y);
    void assignPixels(int n_x, int n_y);
    double slope();     //infinity???

    void draw(vector<vector<Color > >&, int, int);

    friend ostream &operator<<(ostream &os, const Line &m);
};


#endif
