#include "Color.h"


using namespace std;

Color::Color() {}

Color::Color(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color(const Color &other)
{
    this->r = other.r;
    this->g = other.g;
    this->b = other.b;
}

Color Color::operator-(Color& rhs) const{
    Color res;

    res.r = this->r - rhs.r;
    res.g = this->g - rhs.g;
    res.b = this->b - rhs.b;

    return res;
}

Color Color::operator+(Color& rhs) const{
    Color res;

    res.r = this->r + rhs.r;
    res.g = this->g + rhs.g;
    res.b = this->b + rhs.b;

    return res;
}

Color Color::operator*(double c) const{
    Color res;

    res.r = this->r * c;
    res.g = this->g * c;
    res.b = this->b * c;

    return res;
}

ostream& operator<<(ostream& os, const Color& c)
{
    os << fixed << setprecision(0) << "rgb(" << c.r << ", " << c.g << ", " << c.b << ")";
    return os;
}
