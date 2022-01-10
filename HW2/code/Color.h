#ifndef __COLOR_H__
#define __COLOR_H__

#include <iostream>
#include <iomanip>

class Color
{
public:
    double r, g, b;

    Color();
    Color(double r, double g, double b);
    Color(const Color &other);

    Color operator-(Color& rhs) const;
    Color operator+(Color& rhs) const;
    Color operator*(double c) const;

    friend std::ostream& operator<<(std::ostream& os, const Color& c);
};

#endif
