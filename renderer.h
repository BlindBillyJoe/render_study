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

    enum Rasterisation
    {
        TRIANGLE,
        SQUARE
    };

    void render(Model* model, TGAImage* image, TGAImage* diffuse, Rasterisation rasterisation = TRIANGLE);
    void line(Vec2i start, Vec2i end, Vec2i uvStart, Vec2i uvEnd, float depth, TGAImage* image, TGAImage* diffuse);
    void line(Vec2i start, Vec2i end, TGAImage* image, const TGAColor& color);
    bool triangle(Triangle tri, TGAImage* image, TGAImage* diffuse, Rasterisation rasteristaion);
    bool triangle(Vec3<Vec3i> points, Vec3<Vec2i> uv, TGAImage* image, TGAImage* diffuse);
    void triangle(Vec3<Vec2i> points, TGAImage* image, const TGAColor& color);

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

    bool squareRasterisation(const Triangle& tri, TGAImage* image, TGAImage* diffuse);
    bool triangleRasterisation(const Triangle& tri, TGAImage* image, TGAImage* diffuse);
};

#endif
