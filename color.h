#ifndef COLOR_H
#define COLOR_H

struct Color
{
    union
    {
        struct { unsigned char r, g, b, a; };
        struct { int val; };
    };

    Color();
    Color(int r, int g, int b, int a);
    Color(int val);

    Color mix(const Color& other);
    Color operator*(float i);
};

#endif //COLOR_H