#include "model.h"

Model::Model()
{

}

Vec3f Model::vert(int idx) const
{
    return m_vertices[idx];
}

std::vector<Vec3f>& Model::vertices()
{
    return m_vertices;
}

int Model::nvertices() const
{
    return m_vertices.size();
}

Veci Model::face(int idx) const
{
    return m_faces[idx];
}

std::vector<Veci>& Model::faces()
{
    return m_faces;
}

int Model::nfaces() const
{
    return m_faces.size();
}