#include "Translation.h"


using namespace std;


Translation::Translation()
{
    this->translationId = -1;
    this->tx = 0.0;
    this->ty = 0.0;
    this->tz = 0.0;
}


Translation::Translation(int translationId, double tx, double ty, double tz)
{
    this->translationId = translationId;
    this->tx = tx;
    this->ty = ty;
    this->tz = tz;
}


Matrix4 Translation::getMatrix() {
    Matrix4 matrix;
    matrix.identity();

    matrix.val[0][3] = this->tx;
    matrix.val[1][3] = this->ty;
    matrix.val[2][3] = this->tz;

    return matrix;
}


ostream &operator<<(ostream &os, const Translation &t)
{
    os << fixed << setprecision(3) << "Translation " << t.translationId << " => [" << t.tx << ", " << t.ty << ", " << t.tz << "]";

    return os;
}