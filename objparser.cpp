#include "objparser.h"
#include "model.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

#include "geometry.h"

using namespace std;

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

    Model* model = new Model();
    string line;
    vector<Vec3f>& vertices = model->vertices();
    vector<Vec3f>& uvs = model->uvs();
    vector<Veci>& faces = model->faces();
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
            Vec3f uv;
            iss >> uv.x >> uv.y >> uv.z;
            uvs.push_back(uv);
        } else if (!line.compare(0, 2, "f ")) {
            vector<Vec2i> f;
            int itrash, idx, uv;
            iss >> trash;
            while (iss >> idx >> trash >> uv >> trash >> itrash) {
                f.push_back({--idx, --uv});
            }
            faces.push_back(f);
        }
    }
    printf("v#: %llu  f#: %llu", vertices.size(), faces.size());
    return model;
}
