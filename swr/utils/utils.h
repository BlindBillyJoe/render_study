#ifndef UTILS_H
#define UTILS_H

#include "geometry.h"
#include "../model/model.h"
#include <algorithm>
#include <cstdio>

namespace utils {
template<typename T>
inline void _sortPoints(T& points)
{
    if (points.first.y > points.second.y)
        std::swap(points.first, points.second);
    else if (points.first.y == points.second.y)
        if(points.first.x > points.second.x)
            std::swap(points.first, points.second);
    if (points.first.y > points.third.y)
        std::swap(points.first, points.third);
    else if (points.first.y == points.third.y)
        if(points.first.x > points.third.x)
            std::swap(points.first, points.third);
    if (points.second.y > points.third.y)
        std::swap(points.second, points.third);
    else if (points.second.y == points.third.y)
        if(points.second.x > points.third.x)
            std::swap(points.second, points.third);
}

template<typename T>
inline void sortPoints(T& points)
{
    _sortPoints(points);
}

template<>
inline void sortPoints(Vec3<Vec3i>& points)
{
    _sortPoints(points);
    // printf("%i:%i %i:%i %i:%i\n", points.first.x, points.first.y, points.second.x, points.second.y, points.third.x, points.third.y);
}

template<>
inline void sortPoints(Vec3<Vec3f>& points)
{
    _sortPoints(points);
    // printf("%f:%f %f:%f %f:%f\n", points.first.x, points.first.y, points.second.x, points.second.y, points.third.x, points.third.y);
}

template<>
inline void sortPoints(Vec3<Vec2i>& points)
{
    _sortPoints(points);
    // printf("%i:%i %i:%i %i:%i\n", points.first.x, points.first.y, points.second.x, points.second.y, points.third.x, points.third.y);
}

template<>
inline void sortPoints(Vec3<Vec2f>& points)
{
    _sortPoints(points);
    // printf("%f:%f %f:%f %f:%f\n", points.first.x, points.first.y, points.second.x, points.second.y, points.third.x, points.third.y);
}

template<>
inline void sortPoints(Triangle& points)
{
    _sortPoints(points);
    // printf("%f:%f %f:%f %f:%f\n", points.first.x, points.first.y, points.second.x, points.second.y, points.third.x, points.third.y);
}

template<typename T>
inline float _getDepth(const Vec2i& point, const Vec3<Vec3<T> >& triangle, bool interpolate)
{
    float depth = 0;
    if(interpolate) {
        float a11 = point.x - triangle.first.x;
        float a12 = triangle.second.x - triangle.first.x;
        float a13 = triangle.third.x - triangle.first.x;
        float a21 = point.y - triangle.first.y;
        float a22 = triangle.second.y - triangle.first.y;
        float a23 = triangle.third.y - triangle.first.y;
        float a32 = triangle.second.z - triangle.first.z;
        float a33 = triangle.third.z - triangle.first.z;

        float b1 = a11 * a22 * a33;
        float b2 = a13 * a21 * a32;
        float b3 = a11 * a23 * a32;
        float b4 = a12 * a21 * a33;

        float c1 = a12 * a23;
        float c2 = a13 * a22;

        float d1 = b1 + b2 - b3 - b4;
        float d2 = c1 - c2;

        depth = triangle.first.z - (d2 == 0 ? 0 : (d1 / d2));

    } else {
        depth = (triangle.first.z + triangle.second.z + triangle.third.z) / 3;
    }
    return depth;
}

template<typename T>
inline float getDepth(const Vec2i& point, const Vec3<Vec3<T> >& triangle, bool interpolate)
{
    retun _getDepth(point, triangle, interpolate);
}

template<>
inline float getDepth(const Vec2i& point, const Vec3<Vec3f>& triangle, bool interpolate)
{
    float depth = _getDepth(point, triangle, interpolate);
    // if(depth < -0)
    //     printf("--------------------\n%f %f %f\n%f %f %f\n%f %f %f\n%i %i %f\n--------------------\n", 
    //     triangle.first.x, triangle.first.y, triangle.first.z, 
    //     triangle.second.x, triangle.second.y, triangle.second.z, 
    //     triangle.third.x, triangle.third.y, triangle.third.z,
    //     point.x, point.y, depth);
    return depth;
}

template<>
inline float getDepth(const Vec2i& point, const Vec3<Vec3i>& triangle, bool interpolate)
{
    float depth = _getDepth(point, triangle, interpolate);
    // if(depth < -0)
    //     printf("--------------------\n%i %i %i\n%i %i %i\n%i %i %i\n%i %i %f\n--------------------\n", 
    //     triangle.first.x, triangle.first.y, triangle.first.z, 
    //     triangle.second.x, triangle.second.y, triangle.second.z, 
    //     triangle.third.x, triangle.third.y, triangle.third.z,
    //     point.x, point.y, depth);
    return depth;
}

inline int round(float d)
{
  return floor(d + 0.5);
}

inline Vec3<Vec3f> round(Vec3<Vec3f> vec)
{
  return {
      { floor(vec.first.x + 0.5), floor(vec.first.y + 0.5), floor(vec.first.z + 0.5) },
      { floor(vec.second.x + 0.5), floor(vec.second.y + 0.5), floor(vec.second.z + 0.5) },
      { floor(vec.third.x + 0.5), floor(vec.third.y + 0.5), floor(vec.third.z + 0.5) },
  };
}

inline bool is_equal(float x, float y)
{
    return std::fabs(x - y) < std::numeric_limits<float>::epsilon();
}

inline int length(const Vec3f& v1, const Vec3f& v2)
{
    return std::sqrt(std::pow(v1.x - v2.x, 2) + std::pow(v1.y - v2.y, 2) + std::pow(v1.z - v2.z, 2));
}

inline void squareRasterTest(Triangle* ptr)
{
    auto f = [](Vec3f a, Vec3f b, Vec3f p) -> float {
        return (p.x - a.x) * (b.y - a.y) - (p.y - a.y)*(b.x - a.x);
    };
    auto is_inside = [=](Vec3f p, Vec3<Vec3f> t) -> bool {
        return f(t.first, t.second, p) * f(t.first, t.second, t.third) >= 0 &&
               f(t.first, t.third, p) * f(t.first, t.third, t.second) >= 0 &&
               f(t.second, t.third, p) * f(t.second, t.third, t.first) >= 0;
    };


    printf(is_inside(ptr->first, ptr->v ) ? "inside " : "not inside\n");
    printf(is_inside(ptr->second, ptr->v ) ? "inside " : "not inside\n");
    printf(is_inside(ptr->third, ptr->v ) ? "inside " : "not inside\n");
}

inline bool is_inside(Vec3f p, Vec3<Vec3f> t)
{
    auto f = [](Vec3f a, Vec3f b, Vec3f p) -> float {
        return (p.x - a.x) * (b.y - a.y) - (p.y - a.y)*(b.x - a.x);
    };

    if(p == t.first || p == t.second || p == t.third) {
        return true;
    }

    auto a = f(t.first, t.second, p) * f(t.first, t.second, t.third);
    auto b = f(t.first, t.third, p)  * f(t.first, t.third, t.second);
    auto c = f(t.second, t.third, p) * f(t.second, t.third, t.first);
    // return a > 0 && b > 0 && c > 0;
    return ((a > 0 || utils::is_equal(a, 0)) && (b > 0 || utils::is_equal(b, 0)) && (c > 0 || utils::is_equal(c, 0)))
            ;
            // || ((a < 0 || utils::is_equal(a, 0)) && (b < 0 || utils::is_equal(b, 0)) && (c < 0 || utils::is_equal(c, 0)));
}

// inline bool is_inside(Vec2i p, Vec3<Vec2i> t)
// {
//     return is_inside( 
//         {p.x, p.y, 0},
//         {
//             { t.x.x, t.x.y, 0 },
//             { t.y.x, t.y.y, 0 },
//             { t.z.x, t.z.y, 0 }
//         });
// }

}

#endif //UTILSH