#include "renderer.h"
#include "tgaimage.h"
#include "model.h"

#include <algorithm>

using namespace std;

int** zBuffer = nullptr;
int h;
int w;

Renderer::Renderer()
{

}

void Renderer::render(Model* model, TGAImage* image, TGAImage* diffuse)
{
    w = image->get_width();
    h = image->get_height();

    zBuffer = new int*[w];
    for (int i = 0; i != w; ++i) {
        zBuffer[i] = new int[h];
        for (int j = 0; j != h; ++j) {
            zBuffer[i][j] = INT_MIN;
        }
    }

    for(const Veci& fVec: model->faces())
    {
        const int z = (model->vert(fVec[0]).z + model->vert(fVec[1]).z + model->vert(fVec[2]).z) * 42;
        triangle(
            {
                { (model->vert(fVec[0]).x + 1) * w / 2, (model->vert(fVec[0]).y + 1) * h / 2},
                { (model->vert(fVec[1]).x + 1) * w / 2, (model->vert(fVec[1]).y + 1) * h / 2},
                { (model->vert(fVec[2]).x + 1) * w / 2, (model->vert(fVec[2]).y + 1) * h / 2}
            },
            z,
            image,
            diffuse
        );
    }

    for (int i = 0; i != w; ++i)
        delete [] zBuffer[i];

    delete[] zBuffer;
    zBuffer = nullptr;
}

void Renderer::line(Vec2i start, Vec2i end, int depth, TGAImage* image, TGAImage* diffuse)
{
    Vec2<const int*> sp, ep;
    bool swaped = false;
    if (std::abs(start.x - end.x) > std::abs(start.y - end.y)) {
        sp = { &start.x, &start.y };
        ep = { &end.x, &end.y };
    } else {
        sp = { &start.y, &start.x };
        ep = { &end.y, &end.x };
        swaped = true;
    }

    if(*sp.first > *ep.first)
    {
        std::swap(sp, ep);
    }

    int dx = std::abs(*sp.x - *ep.x);
    int dy = std::abs(*sp.y - *ep.y);
    int err = 0;
    int derr = dy;
    int y = *sp.y;
    int dirry = *ep.y - *sp.y;
    dirry = dirry > 0 ? 1 : -1;
    float wDiff = diffuse->get_width() / (float)w;
    float hDiff = diffuse->get_height() / (float)h;
    printf("%f %f\n", wDiff, hDiff);
    for (int x = *sp.x; x <= *ep.x; ++x) {
        if(swaped) {
            if (zBuffer) {
                    if( zBuffer[y][x] > depth)
                        continue;
                    zBuffer[y][x] = depth;
            }
            TGAColor diff = diffuse->get(y * wDiff, x * hDiff);
            image->set(y, x, TGAColor(diff.r + depth, diff.g + depth, diff.b + depth, 255));
        } else {
            if (zBuffer) {
                    if( zBuffer[x][y] > depth)
                        continue;
                    zBuffer[x][y] = depth;
            }
            TGAColor diff = diffuse->get(x * wDiff, y * hDiff);
            image->set(x, y, TGAColor(diff.r + depth, diff.g + depth, diff.b + depth, 255));
        }
        err += derr;
        if (err * 2 > dx) {
            y += dirry;
            err -= dx;
        }
    }
}

static void sortPoints(Vec3<Vec2i>& points)
{
    if (points.first.y > points.second.y)
        std::swap(points.first, points.second);
    if (points.first.y > points.third.y)
        std::swap(points.first, points.third);
    if (points.second.y > points.third.y)
        std::swap(points.second, points.third);
}

void Renderer::triangle(Vec3<Vec2i> points, int depth, TGAImage* image, TGAImage* diffuse)
{
    sortPoints(points);

    int totalHeight = points.third.y - points.first.y;
    int firstSegmentHeight = points.second.y - points.first.y + 1;
    int secondSegmentHeight = points.third.y - points.second.y + 1;

    for(int i = 0; i <= totalHeight; ++i)
    {
        bool second_segment = i > firstSegmentHeight - 1 || points.second.y == points.first.y;
        float alpha = (float)i/totalHeight;
        float beta = (float)(i - (second_segment ? points.second.y - points.first.y : 0))/(second_segment ? secondSegmentHeight : firstSegmentHeight);
        Vec2i A = points.first + (points.third - points.first)*alpha;
        Vec2i B = second_segment ? 
            points.second + (points.third - points.second)*beta :
            points.first + (points.second - points.first)*beta;
        line({A.x, points.first.y + i}, {B.x, points.first.y + i}, depth, image, diffuse);
    }
}