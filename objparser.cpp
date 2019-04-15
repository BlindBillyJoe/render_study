#include "objparser.h"
#include "utils.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <regex>

using namespace std;

std::vector<Triangle> triangulate(const Veci& fVec, const vector<Vec3f>& vertices, const vector<Vec2f>& uvs)
{
    std::vector<Triangle> result;
    if (fVec.size() == 4) {
        auto v1 = vertices[fVec[0].first];
        auto vt1 = uvs.size() <= fVec[0].first ? Vec2f() : uvs[fVec[0].first];
        std::vector<std::pair<Vec3f, Vec2f> > buff;
        auto max_dv = make_pair(make_pair(v1, vt1), make_pair(Vec3f{ 0, 0, 0 }, Vec2f{ 0, 0 }));
        int maxLength = 0;
        for (int i = 0; i != 4; ++i) {
            auto v = vertices[fVec[i].first];
            auto vt = uvs.size() <= fVec[i].second ? Vec2f() : uvs[fVec[i].second];
            int l = utils::length(v1, v);
            if (maxLength < l) {
                if(maxLength != 0)
                    buff.push_back(make_pair(v, vt));
                maxLength = l;
                max_dv.second.first = v;
                max_dv.second.second = vt;
            } else {
                buff.push_back(make_pair(v, vt));
            }
        }
        Triangle tri = {
            { 
                max_dv.first.first,
                max_dv.second.first,
                buff[0].first
            },
            {
                max_dv.first.second,
                max_dv.second.second,
                buff[0].second
            }
        };

        result.push_back(tri);

        tri.third = buff[1].first;
        tri.vt.third = buff[1].second;
        result.push_back(tri);
    } else {
        result.push_back({
            {
                vertices[fVec[0].first],
                vertices[fVec[1].first],
                vertices[fVec[2].first]
            },
            {
                uvs.size() <= fVec[0].second ? Vec2f() : uvs[fVec[0].second],
                uvs.size() <= fVec[1].second ? Vec2f() : uvs[fVec[1].second],
                uvs.size() <= fVec[2].second ? Vec2f() : uvs[fVec[2].second],
            }
        });
    }
    return result;
}

ObjParser::ObjParser()
    : Parser()
{

}

Model* ObjParser::parse(const std::string& filename)
{
    ifstream in;
    in.open(filename, ifstream::in);

    if(in.fail())
        return nullptr;

    std::regex re("(\\d+)\\/(\\d+)?\\/?(\\d+)?", std::regex_constants::ECMAScript | std::regex_constants::icase);

    Model* model = new Model();
    string line;
    vector<Vec3f>& vertices = model->vertices();
    vector<Vec2f>& uvs = model->uvs();
    vector<Veci>& faces = model->faces();
    vector<Triangle>& triangles = model->triangles();
    char trash;
    while(!in.eof()) {
        getline(in, line);
        istringstream iss(line.c_str());
        if(!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            iss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } else if (!line.compare(0, 3, "vt ")) {
            iss >> trash >> trash;
            Vec2f uv;
            iss >> uv.x >> uv.y >> trash;
            uvs.push_back(uv);
        } else if (!line.compare(0, 2, "f ")) {
            vector<Vec2i> f;
            auto end = std::sregex_iterator();
            for (std::sregex_iterator it = std::sregex_iterator(line.begin(), line.end(), re); it != end; ++it) {
                f.push_back({ atoi((*it)[1].str().c_str()) - 1, (*it)[2].str().empty() ? 0 : (atoi((*it)[2].str().c_str()) - 1) });
            }
            faces.push_back(f);
            auto tVec = triangulate(f, vertices, uvs);
            triangles.insert(triangles.end(), tVec.begin(), tVec.end());
        }
    }
    printf("v#: %llu  f#: %llu t#: %llu\n", vertices.size(), faces.size(), triangles.size());
    return model;
}
