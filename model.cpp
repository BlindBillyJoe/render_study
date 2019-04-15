#include "model.h"

Model::Model()
{

}

Vec3f Model::vert(size_t idx) const
{
    return m_vertices[idx];
}

std::vector<Vec3f>& Model::vertices()
{
    return m_vertices;
}

size_t Model::nvertices() const
{
    return m_vertices.size();
}

Veci Model::face(size_t idx) const
{
    return m_faces[idx];
}

std::vector<Veci>& Model::faces()
{
    return m_faces;
}

size_t Model::nfaces() const
{
    return m_faces.size();
}

Vec2f Model::uv(size_t idx) const
{
    return m_uv[idx];
}

std::vector<Vec2f>& Model::uvs()
{
    return m_uv;
}

size_t Model::nuvs() const
{
    return m_uv.size();
}

Triangle Model::triangle(size_t idx) const
{
    return m_triangles[idx];
}

std::vector<Triangle>& Model::triangles()
{
    return m_triangles;
}

size_t Model::ntriangles() const
{
    return m_triangles.size();
}