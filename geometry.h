#ifndef GEOMETRY_H
#define GEOMETRY_H

template <typename type>
struct Vec2
{
    union
    {
        struct { type x, y; };
        struct { type first, second; };
    };

    Vec2<type>() : x(type()), y(type()) {}
    Vec2<type>(type _x, type _y) : x(_x), y(_y) {}

    Vec2<type> operator + (const Vec2<type> other) const { return {x + other.x, y + other.y }; }
    Vec2<type> operator - (const Vec2<type> other) const { return {x - other.x, y - other.y }; }

    template<typename T>
    Vec2<type> operator * (T t) { return {x * t, y * t }; }
};


template<typename type>
struct Vec3
{
    union
    {
        struct { type x, y, z; };
        struct { type first, second, third; };
        type raw[3];
    };

    Vec3<type>() : x(type()), y(type()), z(type()) {}
    Vec3<type>(type _x, type _y, type _z) : x(_x), y(_y), z(_z) {}
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2u;

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec3<unsigned int> Vec3u;

typedef Vec2<Vec2i> Line;

#endif //GEOMETRY_H