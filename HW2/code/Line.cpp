#include "Line.h"

Line::Line(){}

Line::Line(Vec4 v0, Vec4 v1) {
    this->v0 = Vec4(v0);
    this->v1 = Vec4(v1);
    this->rejected = false;
}

bool Line::visible(double den, double num, double &t_e, double &t_l){
    double t = num/den;

    if (den > 0) {        // pot. entering
        if (t > t_l) return false;
        if (t > t_e) t_e = t;
    }
    else if (den < 0) {     // pot. leaving
        if (t < t_e) return false;
        if (t < t_l) t_l = t;
    }
    else if (num > 0) 
        return false;

    return true;
}

void Line::clip(double x_min, double x_max, double y_min, double y_max, double z_min, double z_max){
    double d_x, d_y, d_z;
    double t_e = 0;
    double t_l = 1;
    
    d_x = v1.x - v0.x;
    d_y = v1.y - v0.y;
    d_z = v1.z - v0.z;

    Color d_color = v1.color - v0.color;

    if (visible(d_x, x_min - this->v0.x, t_e, t_l) && visible(-d_x, this->v0.x - x_max, t_e, t_l) &&
        visible(d_y, y_min - this->v0.y, t_e, t_l) && visible(-d_y, this->v0.y - y_max, t_e, t_l) && 
        visible(d_z, z_min - this->v0.z, t_e, t_l) && visible(-d_z, this->v0.z - z_max, t_e, t_l)){

        if (t_l < 1){
            v1.x = v0.x + d_x * t_l;
            v1.y = v0.y + d_y * t_l;
            v1.z = v0.z + d_z * t_l;

            v1.color = v0.color + (d_color * t_l);
        }
        if (t_e > 0){
            v0.x = v0.x + d_x * t_e;
            v0.y = v0.y + d_y * t_e;
            v0.z = v0.z + d_z * t_e;

            v0.color = v0.color + (d_color * t_e);
        }
    }
    else
        this->rejected = true;

}

double Line::f(double x, double y){
    int x0 = this->v0.x;
    int y0 = this->v0.y;
    int x1 = this->v1.x;
    int y1 = this->v1.y;

    return x * (y0 - y1) + y * (x1- x0) + x0*y1 - y0*x1;
}

double Line::slope(){
    if (this->v1.x == this->v0.x){
        if (this->v1.y > this->v0.y)
            return DBL_MAX;
        else
            return -DBL_MAX;
    }

    return (this->v1.y - this->v0.y) / (this->v1.x - this->v0.x);
}

void Line::draw(vector<vector<Color > >& image, int n_x, int n_y){

    if (this->rejected)
        return;

    double slope = this->slope();

    this->assignPixels(n_x, n_y);

    int x, y;
    Color acc = this->v0.color;
    Color d_color = this->v1.color - this->v0.color;

    if (slope > 0 && slope <= 1){
        y = this->v0.y; // y = y0
        d_color = d_color * (1 / ABS(this->v0.x - this->v1.x));

        if (this->v1.y > this->v0.y){   // trivial one
            for (x = this->v0.x; x <= this->v1.x; x++){
                image[x][y] = acc; //Color(0, 0, 0);

                if (f(x+1, y+0.5) < 0)
                    y++;

                acc = acc + d_color;
            }
        }
        else{   // 2nd
            for (x = this->v0.x; x >= this->v1.x; x--){
                image[x][y] = acc;  //Color(0, 0, 0);

                if (f(x-1, y-0.5) < 0)
                    y--;

                acc = acc + d_color;
            }
        }
    }
    else if (slope > 1 && slope <= DBL_MAX){
        x = this->v0.x;
        d_color = d_color * (1 / ABS(this->v0.y - this->v1.y));

        if (this->v1.y > this->v0.y){   // 3rd
            for (y = this->v0.y; y <= this->v1.y; y++){
                image[x][y] = acc; //Color(0, 0, 0);

                if (f(x+0.5, y+1) > 0)
                    x++;

                acc = acc + d_color;
            }
        }
        else{       //4th
            for (y = this->v0.y; y >= this->v1.y; y--){
                image[x][y] = acc; //Color(0, 0, 0);

                if (f(x-0.5, y-1) > 0)
                    x--;

                acc = acc + d_color;
            }
        }
    }
    else if (slope <= -1){
        x = this->v0.x;
        d_color = d_color * (1 / ABS(this->v0.y - this->v1.y));

        if (this->v1.y > this->v0.y){   // 5th
            for (y = this->v0.y; y <= this->v1.y; y++){
                image[x][y] = acc; //Color(0, 0, 0);

                if (f(x-0.5, y+1) < 0)
                    x--;

                acc = acc + d_color;
            }
        }
        else{       // 6th
            for (y = this->v0.y; y >= this->v1.y; y--){
                image[x][y] = acc;  //Color(0, 0, 0);

                if (f(x+0.5, y-1) < 0)
                    x++;
                
                acc = acc + d_color;
            }
        }
    }
    else if (slope > -1){
        y = this->v0.y;
        d_color = d_color * (1 / ABS(this->v0.x - this->v1.x));

        if (this->v1.y > this->v0.y){   // 7th
            for (x = this->v0.x; x >= this->v1.x; x--){
                image[x][y] = acc;  //Color(0, 0, 0);

                if (f(x-1, y+0.5) > 0)
                    y++;

                acc = acc + d_color;
            }
        }
        else{   // 8th
            for (x = this->v0.x; x <= this->v1.x; x++){
                image[x][y] = acc;  //Color(0, 0, 0);

                if (f(x+1, y-0.5) > 0)
                    y--;

                acc = acc + d_color;
            }
        }
    }
}

void Line::assignPixels(int n_x, int n_y){
    this->v0.x = floor(this->v0.x + .5);
    this->v0.y = floor(this->v0.y + .5);

    this->v1.x = floor(this->v1.x + .5);
    this->v1.y = floor(this->v1.y + .5);

    if (this->v0.x == n_x)
        this->v0.x -= 1;

    if (this->v0.y == n_y)
        this->v0.y -= 1;

    if (this->v1.x == n_x)
        this->v1.x -= 1;
    
    if (this->v1.y == n_y)
        this->v1.y -= 1;
}

ostream& operator<<(ostream& os, const Line& line) {
    
    os << fixed << setprecision(6) << "Endpoint 1: " << line.v0 << ", Endpoint 2: " << line.v1 << endl;;

    return os;
}
