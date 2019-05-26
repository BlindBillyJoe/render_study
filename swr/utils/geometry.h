#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <algorithm>
#include <cmath>

inline bool is_equal(float x, float y)
{
    return std::fabs(x - y) < std::numeric_limits<float>::epsilon();
}

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

    template<typename T>
    Vec2<type> operator + (const Vec2<T> other) const { return {x + other.x, y + other.y }; }

    template<typename T>
    Vec2<type> operator - (const Vec2<T> other) const { return {x - other.x, y - other.y }; }

    template<typename T>
    Vec2<type> operator * (Vec2<T> t) const { return { x * t.x, y * t.y }; }

    template<typename T>
    Vec2<type> operator * (T t) const { return {x * t, y * t }; }

    bool operator == (const Vec2<type>& vec) const { return (x == vec.x && y == vec.y); }
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

    Vec3<type> operator + (const Vec3<type>& other) const { return {x + other.x, y + other.y, z + other.z }; }
    Vec3<type> operator - (const Vec3<type>& other) const { return {x - other.x, y - other.y, z - other.z }; }
    Vec3<type> operator * (const Vec3<type>& t) const { return { x * t.x, y * t.y, z * t.z }; }

    template<typename T>
    Vec3<type> operator + (T t) const { return {x + t, y + t, z + t }; }
    template<typename T>
    Vec3<type> operator - (T t) const { return { x - t, y - t, z - t }; }
    template<typename T>
    Vec3<type> operator * (const T& t) const { return {x * t, y * t, z * t }; }

    bool operator == (const Vec3<float>& vec) { return (is_equal(x, vec.x) && is_equal(y, vec.y) && is_equal(z, vec.z)); }
    bool operator == (const Vec3<type>& vec) const { return (x == vec.x && y == vec.y && z == vec.z); }
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec2<unsigned int> Vec2u;

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;
typedef Vec3<unsigned int> Vec3u;

typedef Vec2<Vec2i> Line;

struct Triangle
{
    Vec3<Vec3f> v;
    Vec3<Vec2f> vt;

    Triangle(Vec3<Vec3f> v, Vec3<Vec2f> vt) : v(v), vt(vt) {}

    template<typename T>
    Triangle operator + (const Vec3<T>& vec) const { return { v + vec, vt }; }

    template<typename T>
    Triangle operator * (const Vec3<T>& vec) const { return { v * vec, vt }; }
};

#endif //GEOMETRY_H