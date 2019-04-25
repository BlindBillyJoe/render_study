#ifndef MODEL_H
#define MODEL_H

#include "../utils/geometry.h"
#include <vector>
#include <stddef.h>

typedef std::vector<Vec2i> Veci;

class Model
{
    std::vector<Vec2f> m_uv;
    std::vector<Vec3f> m_vertices;
    std::vector<Veci> m_faces;
    std::vector<Triangle> m_triangles;

public:
    Model();

    Vec3f vert(size_t idx) const;
    std::vector<Vec3f>& vertices();
    size_t nvertices() const;

    Veci face(size_t idx) const;
    std::vector<Veci>& faces();
    size_t nfaces() const;

    Vec2f uv(size_t idx) const;
    std::vector<Vec2f>& uvs();
    size_t nuvs() const;

    Triangle triangle(size_t idx) const;
    std::vector<Triangle>& triangles();
    size_t ntriangles() const;
};

#endif
