#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "geometry.h"

class Model;
class TGAImage;
struct TGAColor;
class Model;
class Renderer {

public:
    Renderer();

    void render(Model* model, TGAImage* image, TGAImage* diffuse);
    void line(Vec2i start, Vec2i end, Vec2i uvStart, Vec2i uvEnd, float depth, TGAImage* image, TGAImage* diffuse);
    void line(Vec2i start, Vec2i end, TGAImage* image, const TGAColor& color);
    bool triangle(Vec3<Vec2i> points, Vec3<Vec2i> uv, float depth, TGAImage* image, TGAImage* diffuse);
    void triangle(Vec3<Vec2i> points, TGAImage* image, const TGAColor& color);
};

#endif
