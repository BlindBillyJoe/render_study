#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "geometry.h"

class Model;
class TGAImage;
class TGAColor;
class Model;
class Renderer {

public:
    Renderer();

    void render(Model* model, TGAImage* image, TGAImage* diffuse);
    void line(Vec2i start, Vec2i end, int depth, TGAImage* image, TGAImage* diffuse);
    void triangle(Vec3<Vec2i> points, int depth, TGAImage* image, TGAImage* diffuse);
};

#endif