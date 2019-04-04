#ifndef MODEL_H
#define MODEL_H

#include "geometry.h"
#include <vector>

typedef std::vector<Vec2i> Veci;

class Model
{
    std::vector<Vec3f> m_uv;
    std::vector<Vec3f> m_vertices;
    std::vector<Veci> m_faces;
public:
    Model();

    Vec3f vert(size_t idx) const;
    std::vector<Vec3f>& vertices();
    size_t nvertices() const;

    Veci face(size_t idx) const;
    std::vector<Veci>& faces();
    size_t nfaces() const;

    Vec3f uv(size_t idx) const;
    std::vector<Vec3f>& uvs();
    size_t nuvs() const;
};

#endif
