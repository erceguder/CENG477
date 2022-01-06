#include "Matrix4.h"


using namespace std;


Matrix4::Matrix4()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->val[i][j] = 0;
        }
    }
}


Matrix4::Matrix4(double val[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->val[i][j] = val[i][j];
        }
    }
}


Matrix4::Matrix4(const Matrix4 &other)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            this->val[i][j] = other.val[i][j];
        }
    }
}


void Matrix4::identity() {

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            
            if (i == j){
                this->val[i][j] = 1.0;
            } else{
                this->val[i][j] = 0.0;
            }
        }
    }

}


Matrix4 Matrix4::operator*(Matrix4 obj) const {
    Matrix4 res;
    double total;

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){

            total = 0;
            for (int k = 0; k < 4; k++)
                total += this->val[i][k] * obj.val[k][j];

            res.val[i][j] = total;
        }
    }

    return res;
}


Vec4 Matrix4::operator*(Vec4 obj) {
    double values[4];
    double total;

    for (int i = 0; i < 4; i++){
        total = 0;
        for (int j = 0; j < 4; j++){
            total += this->val[i][j] * obj.get(j);
        }
        values[i] = total;
    }

    return Vec4(values[0], values[1], values[2], values[3], obj.colorId);

}


ostream &operator<<(ostream &os, const Matrix4 &m)
{

    os << fixed << setprecision(6) << "|" << m.val[0][0] << "|" << m.val[0][1] << "|" << m.val[0][2] << "|" << m.val[0][3] << "|"
       << endl
       << "|" << m.val[1][0] << "|" << m.val[1][1] << "|" << m.val[1][2] << "|" << m.val[1][3] << "|"
       << endl
       << "|" << m.val[2][0] << "|" << m.val[2][1] << "|" << m.val[2][2] << "|" << m.val[2][3] << "|"
       << endl
       << "|" << m.val[3][0] << "|" << m.val[3][1] << "|" << m.val[3][2] << "|" << m.val[3][3] << "|";

    return os;
}