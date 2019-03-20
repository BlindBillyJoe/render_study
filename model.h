#ifndef MODEL_H
#define MODEL_H

#include "geometry.h"
#include <vector>

typedef std::vector<int> Veci;

class Model
{
    std::vector<Vec3f> m_vertices;
    std::vector<Veci> m_faces;
public:
    Model();

    Vec3f vert(int idx) const;
    std::vector<Vec3f>& vertices();
    int nvertices() const;

    Veci face(int idx) const;
    std::vector<Veci>& faces();
    int nfaces() const;
};

#endif