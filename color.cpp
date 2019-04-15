#include "color.h"

Color::Color()
    : val(0)
{

}

Color::Color(int r, int g, int b, int a)
    : r(r), g(g), b(b), a(a)
{

}

Color::Color(int val)
    : val(val)
{

}

Color Color::mix(const Color& other)
{
    return Color(other.r / 2 + r / 2, other.g / 2 + g / 2, other.b / 2 + b / 2, other.a / 2 + a / 2);
}

Color Color::operator*(float i)
{
    return Color(r * i, g * i, b * i, a);
}