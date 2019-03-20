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
    Model* model = new Model();
    ifstream in;
    in.open(filename, ifstream::in);

    if(in.fail())
        return nullptr;

    string line;
    vector<Vec3f>& vertices = model->vertices();
    vector<Veci>& faces = model->faces();
    char trash;
    while(!in.eof()) {
        getline(in, line);
        istringstream iss(line.c_str());
        if(!line.compare(0, 2, "v ")) {
            iss >> trash;
            Vec3f v;
            for (int i=0; i<3; i++) {
                iss >> v.raw[i];
            }
            vertices.push_back(v);
        } else if (!line.compare(0, 2, "f ")) {
            vector<int> f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                f.push_back(--idx);
            }
            faces.push_back(f);
        }
    }
    printf("v#: %llu  f#: %llu", vertices.size(), faces.size());
    return model;
}