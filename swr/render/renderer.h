#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

#include "../utils/geometry.h"

class Model;
class IODevice;
struct Color;
class Model;
class Renderer {

public:
    Renderer();

    void render(Model* model, IODevice* image, IODevice* diffuse);
    void line(Vec2i start, Vec2i end, Vec2i uvStart, Vec2i uvEnd, float depth, IODevice* image, IODevice* diffuse);
    void line(Vec2i start, Vec2i end, IODevice* image, const Color& color);
    bool triangle(Triangle tri, IODevice* image, IODevice* diffuse);
    bool triangle(Vec3<Vec3i> points, Vec3<Vec2i> uv, IODevice* image, IODevice* diffuse);
    void triangle(Vec3<Vec2i> points, IODevice* image, const Color& color);

private:
    void preprocess(Model* model);

private:
    float** zBuffer;
    int h;
    int w;
    int uvH;
    int uvW;

    Vec3f step;
    Vec3f shift;

    void createBuffer();
    void deleteBuffer();

    bool triangleRasterisation(const Triangle& tri, IODevice* image, IODevice* diffuse);
};

#endif
