#include "renderer.h"
#include "tgaimage.h"
#include "model.h"

#include <algorithm>

using namespace std;

static float** zBuffer = nullptr;
static int h;
static int w;
static int uvH;
static int uvW;

void line_optimized(Vec3i xxy, TGAImage* output, const TGAColor& color);

Renderer::Renderer()
{

}

void Renderer::render(Model* model, TGAImage* image, TGAImage* diffuse)
{
    w = image->get_width();
    h = image->get_height();
    uvW = diffuse->get_width();
    uvH = diffuse->get_height();

    zBuffer = new float*[w];
    for (int i = 0; i != w; ++i) {
        zBuffer[i] = new float[h];
        for (int j = 0; j != h; ++j) {
            zBuffer[i][j] = -1;
        }
    }

    if(!model)
        return;

    for(const Veci& fVec: model->faces())
    {
        const float z = (
                model->vert(static_cast<size_t>(fVec[0].first)).z +
                model->vert(static_cast<size_t>(fVec[1].first)).z +
                model->vert(static_cast<size_t>(fVec[2].first)).z +
                3.f) / 6.f;
        bool result = triangle(
            {
                {
                    static_cast<int>((model->vert(static_cast<size_t>(fVec[0].first)).x + 1) * w / 2),
                    static_cast<int>((model->vert(static_cast<size_t>(fVec[0].first)).y + 1) * h / 2)
                },
                {
                    static_cast<int>((model->vert(static_cast<size_t>(fVec[1].first)).x + 1) * w / 2),
                    static_cast<int>((model->vert(static_cast<size_t>(fVec[1].first)).y + 1) * h / 2)
                },
                {
                    static_cast<int>((model->vert(static_cast<size_t>(fVec[2].first)).x + 1) * w / 2),
                    static_cast<int>((model->vert(static_cast<size_t>(fVec[2].first)).y + 1) * h / 2)
                }
            },
            {
                {
                    static_cast<int>((model->uv(static_cast<size_t>(fVec[0].second)).x) * uvW),
                    static_cast<int>((model->uv(static_cast<size_t>(fVec[0].second)).y) * uvH)
                },
                {
                    static_cast<int>((model->uv(static_cast<size_t>(fVec[1].second)).x) * uvW),
                    static_cast<int>((model->uv(static_cast<size_t>(fVec[1].second)).y) * uvH)
                },
                {
                    static_cast<int>((model->uv(static_cast<size_t>(fVec[2].second)).x) * uvW),
                    static_cast<int>((model->uv(static_cast<size_t>(fVec[2].second)).y) * uvH)
                }
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

void Renderer::line(Vec2i start, Vec2i end, Vec2i uvStart, Vec2i uvEnd, float depth, TGAImage* image, TGAImage* diffuse)
{
    Vec2<const int*> sp, ep;
    Vec2<const int*> suv, euv;
    bool swaped = false, uvSwaped = false;
    if (std::abs(start.x - end.x) > std::abs(start.y - end.y)) {
        sp = { &start.x, &start.y };
        ep = { &end.x, &end.y };
    } else {
        sp = { &start.y, &start.x };
        ep = { &end.y, &end.x };
        swaped = true;
    }

    if (std::abs(uvStart.x - uvEnd.x) > std::abs(uvStart.y - uvEnd.y))
    {
        suv = { &uvStart.x, &uvStart.y };
        euv = { &uvEnd.x, &uvEnd.y };
    } else {
        suv = { &uvStart.y, &uvStart.x };
        euv = { &uvEnd.y, &uvEnd.x };
        uvSwaped = true;
    }

    if(*sp.first > *ep.first)
    {
        std::swap(sp, ep);
    }
    if(*suv.first > *euv.first)
    {
        std::swap(suv, euv);
    }

    int dxuv = std::abs(*suv.x - *euv.x);
    int dyuv = std::abs(*suv.y - *euv.y);
    int erruv = 0;
    int derruv = dyuv;
    int yuv = *suv.y;
    int dirryuv = *euv.y - *suv.y;
    dirryuv = dirryuv > 0 ? 1 : -1;

    int dx = std::abs(*sp.x - *ep.x);
    int dy = std::abs(*sp.y - *ep.y);
    int err = 0;
    int derr = dy;
    int y = *sp.y;
    int dirry = *ep.y - *sp.y;
    dirry = dirry > 0 ? 1 : -1;

    double difference = (float)(std::abs((uvSwaped ? *suv.y : *suv.x) - (uvSwaped ? *euv.y : *euv.x))) / std::abs(((swaped ? *sp.y : *sp.x) - (swaped ? *ep.y : *ep.x)));
    int xuv = *suv.x;
    // printf("%lf\n", difference);
    for (int x = *sp.x; x <= *ep.x; ++x, ++xuv) {
        int &a = swaped ? y : x;
        int &b = swaped ? x : y;
        const int &c = uvSwaped ? yuv : xuv;
        const int &d = uvSwaped ? xuv : yuv;
        
        if (zBuffer) {
                if( zBuffer[a][b] > depth)
                    continue;
                zBuffer[a][b] = depth;
        }
        TGAColor diff = diffuse->get(c, d);
        // printf("%i:%i %i:%i %f\n", a, b, c, d, difference);
        image->set(
                    a, b,
                    TGAColor(
                        static_cast<unsigned char>(diff.r*depth),
                        static_cast<unsigned char>(diff.g*depth),
                        static_cast<unsigned char>(diff.b*depth),
                        255
                        )
                    );
        err += derr;
        if (err * 2 > dx) {
            y += dirry;
            err -= dx;
        }
        if (erruv * 2 > dxuv) {
            yuv += dirryuv;
            erruv -= dxuv;
        }
    }
}

void Renderer::line(Vec2i start, Vec2i end, TGAImage* image, const TGAColor& color)
{
    Vec2<const int*> sp, ep;
    bool swaped = false, uvSwaped = false;
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
    for (int x = *sp.x; x <= *ep.x; ++x) {
        int &a = swaped ? y : x;
        int &b = swaped ? x : y;
        image->set(a, b, color);
        err += derr;
        if (err * 2 > dx) {
            y += dirry;
            err -= dx;
        }
    }
}

static void sortPoints(Vec3<Vec2i>& points, string type)
{
    if (points.first.y > points.second.y)
        std::swap(points.first, points.second);
    if (points.first.y > points.third.y)
        std::swap(points.first, points.third);
    if (points.second.y > points.third.y)
        std::swap(points.second, points.third);
    // printf("%i:%i %i:%i %i:%i %s\n", points.first.x, points.first.y, points.second.x, points.second.y, points.third.x, points.third.y, type.c_str());
}

bool Renderer::triangle(Vec3<Vec2i> v_p, Vec3<Vec2i> uv_p, float depth, TGAImage* image, TGAImage* diffuse)
{
    sortPoints(v_p, "triangle");
    sortPoints(uv_p, "uv");
    int totalHeight = v_p.third.y - v_p.first.y;
    int uvTotalHeight = uv_p.third.y - uv_p.first.y;
    if(!totalHeight)
        return false;
    int firstSegmentHeight = v_p.second.y - v_p.first.y + 1;
    int secondSegmentHeight = v_p.third.y - v_p.second.y + 1;

    float uv_diff_y = uvTotalHeight / static_cast<float>(totalHeight);
    float uvy = uv_p.first.y;

    for(int i = 0; i <= totalHeight; ++i)
    {
        bool second_segment = i > firstSegmentHeight - 1 || v_p.second.y == v_p.first.y;
        float alpha = static_cast<float>(i)/totalHeight;
        float beta = static_cast<float>(i - (second_segment ? v_p.second.y - v_p.first.y : 0)) /
            (second_segment ? secondSegmentHeight : firstSegmentHeight);
        float uvAlpha = static_cast<float>(i) / uvTotalHeight;
        float uvBeta = static_cast<float>(i - (second_segment ? uv_p.second.y - uv_p.first.y : 0)) /
            (second_segment ? secondSegmentHeight : firstSegmentHeight);
        Vec3i v = {
            v_p.first.x + (v_p.third.x - v_p.first.x) * alpha,
            second_segment ?
            v_p.second.x + (v_p.third.x - v_p.second.x) * beta :
            v_p.first.x + (v_p.second.x - v_p.first.x) * beta,
            v_p.first.y
        };
        Vec3i uv = {
            uv_p.first.x + (uv_p.third.x - uv_p.first.x) * uvAlpha,
            second_segment ?
            uv_p.second.x + (uv_p.third.x - uv_p.second.x) * uvBeta :
            uv_p.first.x + (uv_p.second.x - uv_p.first.x) * uvBeta,
            uv_p.first.y
        };
        if (v.first > v.second)
            std::swap(v.first, v.second);
        if (uv.first > uv.second)
            std::swap(uv.first, uv.second);
        float uv_diff_x = static_cast<float>(uv.first == uv.second ? 1 : (uv.second - uv.first)) / 
            (v.first == v.second ? 1 : (v.second - v.first));
        float uvx = uv.first;
        int x = v.first;
        int y = v.third + i;
        for (; x <= v.second; ++x, uvx += uv_diff_x) {
            if (zBuffer[x][y] > depth)
                continue;
            zBuffer[x][y] = depth;
            TGAColor col = diffuse->get(static_cast<int>(uvx), uvy);
            image->set(x, y, diffuse->get(uvx, uvy) * depth);
        }
        uvy += uv_diff_y;
    }
    return true;
}

void Renderer::triangle(Vec3<Vec2i> points, TGAImage* image, const TGAColor& color)
{
    sortPoints(points, "triangle");
    int totalHeight = points.third.y - points.first.y;
    if(!totalHeight)
        return;
    int firstSegmentHeight = points.second.y - points.first.y + 1;
    int secondSegmentHeight = points.third.y - points.second.y + 1;
    for(int i = 0; i <= totalHeight; ++i)
    {
        bool second_segment = i > firstSegmentHeight - 1 || points.second.y == points.first.y;
        float alpha = static_cast<float>(i)/totalHeight;
        float beta = static_cast<float>(i - (second_segment ? points.second.y - points.first.y : 0))/(second_segment ? secondSegmentHeight : firstSegmentHeight);
        Vec2i A = points.first + (points.third - points.first)*alpha;
        Vec2i B = second_segment ?
            points.second + (points.third - points.second)*beta :
            points.first + (points.second - points.first)*beta;
        line_optimized(
            { A.x, B.x, points.first.y + i},
            image, color
        );
    }
}

void line_optimized(Vec3i xxy, TGAImage* output, const TGAColor& color)
{
    if(xxy.x > xxy.y)
        std::swap(xxy.x, xxy.y);
    for(int x = xxy.first; x <= xxy.second; ++x) {
        output->set(x, xxy.third, color);
    }
}