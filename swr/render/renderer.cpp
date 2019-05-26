#include "renderer.h"
#include "../render/iodevice/iodevice.h"
#include "../model/model.h"
#include "../utils/utils.h"
#include "../utils/color.h"

#include <algorithm>
#include <iostream>

namespace colors
{
    Color white = Color(255, 255, 255, 255);
    Color black = Color(0, 0, 0, 0);

    Color red = Color(255, 0, 0, 255);
    Color green = Color(0, 255, 0, 255);
    Color blue = Color(0, 0, 255, 255);
}

static void line_optimized(Vec3i xxy, IODevice* output, const Color& color)
{
    if(xxy.x > xxy.y)
        std::swap(xxy.x, xxy.y);
    for(int x = xxy.first; x <= xxy.second; ++x) {
        output->set(x, xxy.third, color);
    }
}

template<typename T>
Vec3f getVec(const T& vec, float alpha, float beta, bool second_segment)
{
   return {
            utils::round(vec.first.x + (vec.third.x - vec.first.x) * alpha),
            second_segment ?
            utils::round(vec.second.x + (vec.third.x - vec.second.x) * beta) :
            utils::round(vec.first.x + (vec.second.x - vec.first.x) * beta),
            utils::round(vec.first.y)
        };
}

Renderer::Renderer()
    : zBuffer(nullptr)
    , h(0)
    , w(0)
    , uvW(0)
    , uvH(0)
    , step({ 0, 0, 0 })
    , shift({ 0, 0, 0 })
{

}

void Renderer::render(Model* model, IODevice* image, IODevice* diffuse)
{
    w = image->width();
    h = image->height();
    uvW = diffuse ? diffuse->width() : 0;
    uvH = diffuse ? diffuse->height() : 0;

    if(!model)
        return;

    createBuffer();
    preprocess(model);
    printf("render start\n");
    for(auto tri: model->triangles()) {
        triangle({ tri.v, tri.vt * Vec2f{uvW, uvH}  }, image, diffuse);
    }
    printf("render end\n");
    deleteBuffer();
}

void Renderer::line(Vec2i start, Vec2i end, Vec2i uvStart, Vec2i uvEnd, float depth, IODevice* image, IODevice* diffuse)
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

    int xuv = *suv.x;
    for (int x = *sp.x; x <= *ep.x; ++x, ++xuv) {
        int &a = swaped ? y : x;
        int &b = swaped ? x : y;
        const int &c = uvSwaped ? yuv : xuv;
        const int &d = uvSwaped ? xuv : yuv;
        
        if (zBuffer) {
            if( zBuffer[a][b] > depth)
                continue;
        }
        Color diff = diffuse->get(c, d);
        image->set(a, b, diff * depth);
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

void Renderer::line(Vec2i start, Vec2i end, IODevice* image, const Color& color)
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

bool Renderer::triangle(Vec3<Vec3i> v_p, Vec3<Vec2i> uv_p, IODevice* image, IODevice* diffuse)
{
    if (v_p.first.x < 0 || v_p.first.y < 0 || v_p.second.x < 0 || v_p.second.y < 0 || v_p.third.x < 0 || v_p.third.y < 0)
        return false;
    int totalHeight = v_p.third.y - v_p.first.y;
    int uvTotalHeight = uv_p.third.y - uv_p.first.y;
    if(!totalHeight)
        return false;
    int firstSegmentHeight = v_p.second.y - v_p.first.y + 1;
    int secondSegmentHeight = v_p.third.y - v_p.second.y + 1;

    float uv_diff_y = uvTotalHeight / static_cast<float>(totalHeight);
    float uvy = uv_p.first.y;
    for(int i = 0; i <= totalHeight; ++i) {
        bool second_segment = i > firstSegmentHeight - 1 || v_p.second.y == v_p.first.y;
        float alpha = static_cast<float>(i)/totalHeight;
        float beta = static_cast<float>(i - (second_segment ? v_p.second.y - v_p.first.y : 0)) /
            (second_segment ? secondSegmentHeight : firstSegmentHeight);
        float uvAlpha = static_cast<float>(i) / uvTotalHeight;
        float uvBeta = static_cast<float>(i - (second_segment ? uv_p.second.y - uv_p.first.y : 0)) /
            (second_segment ? secondSegmentHeight : firstSegmentHeight);
        Vec3f v = getVec(v_p, alpha, beta, second_segment);
        Vec3f uv = getVec(uv_p, uvAlpha, uvBeta, second_segment);
        if (v.first > v.second)
            std::swap(v.first, v.second);
        if (uv.first > uv.second)
            std::swap(uv.first, uv.second);
        float uv_diff_x = (uv.first == uv.second ? 1 : (uv.second - uv.first)) / 
            (utils::is_equal(v.first, v.second) ? 1 : (v.second - v.first));
        float uvx = uv.first;
        int x = v.first;
        int y = v.third + i;
        for (; x <= v.second; ++x, uvx += uv_diff_x) {
            float depth = utils::getDepth({x, y}, v_p, false);
            if (zBuffer[x][y] > depth)
                continue;
            zBuffer[x][y] = depth;
            Color col = diffuse ? diffuse->get(static_cast<int>(uvx), uvy) : Color(255, 255, 255, 255);
            image->set(x, y, col * (depth / 255.));
        }
        uvy += uv_diff_y;
    }

    return true;
}

bool Renderer::triangle(Triangle tri, IODevice* image, IODevice* diffuse)
{
    if (tri.v.first.x < 0 || tri.v.first.y < 0 || tri.v.second.x < 0 || tri.v.second.y < 0 || tri.v.third.x < 0 || tri.v.third.y < 0)
        return false;

    utils::sortPoints(tri.v);
    utils::sortPoints(tri.vt);

    triangleRasterisation(tri, image, diffuse);

    return true;
}

void Renderer::triangle(Vec3<Vec2i> points, IODevice* image, const Color& color)
{
    utils::sortPoints(points);
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

void Renderer::preprocess(Model* model)
{
    Vec3f min = { 0, 0, 0 };
    Vec3f max = { 0, 0, 0 };
    for (const auto& vert: model->vertices()) {
        min = {
            min.x < vert.x ? min.x : vert.x,
            min.y < vert.y ? min.y : vert.y,
            min.z < vert.z ? min.z : vert.z
        };
        max = {
            max.x > vert.x ? max.x : vert.x,
            max.y > vert.y ? max.y : vert.y,
            max.z > vert.z ? max.z : vert.z
        };
    }

    shift = {
        -min.x,
        -min.y,
        -min.z
    };

    printf("shift: %f/%f/%f\n", shift.x, shift.y, shift.z);

    min = shift + min;
    max = shift + max;
    
    printf("min/max: %f/%f/%f %f/%f/%f\n", min.x, min.y, min.z, max.x, max.y, max.z);
    
    step = {
        w / max.x,
        h / max.y,
        200 / max.z
    };

    if (step.x > step.y)
        step.x = step.y;
    else
        step.y = step.x;

    printf("step: %f/%f/%f\n", step.x, step.y, step.z);

    for (auto& vert: model->vertices()) {
        int x = utils::round((vert.x + shift.x) * step.x);
        int y = utils::round((vert.y + shift.y) * step.y);
        float& cell_1 = zBuffer[x][y];
        float z = (vert.z + shift.z) * step.z;
        cell_1 = cell_1 < z ? z : cell_1;
        vert.x = x;
        vert.y = y;
        vert.z = z;
    }
    printf("vertices ok\n");
    int found = 0;
    for(auto& t: model->triangles()) {
        t.v.first.x     = utils::round((t.v.first.x  + shift.x) * step.x);
        t.v.second.x    = utils::round((t.v.second.x + shift.x) * step.x);
        t.v.third.x     = utils::round((t.v.third.x  + shift.x) * step.x);
        t.v.first.y     = utils::round((t.v.first.y  + shift.y) * step.y);
        t.v.second.y    = utils::round((t.v.second.y + shift.y) * step.y);
        t.v.third.y     = utils::round((t.v.third.y  + shift.y) * step.y);
        t.v.first.z     =             ((t.v.first.z  + shift.z) * step.z);
        t.v.second.z    =             ((t.v.second.z + shift.z) * step.z);
        t.v.third.z     =             ((t.v.third.z  + shift.z) * step.z);
    }
    printf("triangles ok\n");
}

void Renderer::createBuffer()
{
    zBuffer = new float*[w + 1];
    for (int i = 0; i != w + 1; ++i) {
        zBuffer[i] = new float[h + 1];
        for (int j = 0; j != h + 1; ++j) {
            zBuffer[i][j] = -1;
        }
    }
}

void Renderer::deleteBuffer()
{
    for (int i = 0; i != w; ++i)
        delete [] zBuffer[i];

    delete[] zBuffer;
    zBuffer = nullptr;
}

bool Renderer::triangleRasterisation(const Triangle& tri, IODevice* image, IODevice* diffuse)
{
    float totalHeight = tri.v.third.y - tri.v.first.y;
    float uvTotalHeight = tri.vt.third.y - tri.vt.first.y;

    if(!totalHeight)
        return false;

    float firstSegmentHeight = utils::is_equal((tri.v.second.y - tri.v.first.y), 0) ? 1 : (tri.v.second.y - tri.v.first.y);
    float secondSegmentHeight = utils::is_equal((tri.v.third.y - tri.v.second.y), 0) ? 1 : (tri.v.third.y - tri.v.second.y);

    float uvFirstSegmentHeight = utils::is_equal((tri.vt.second.y - tri.vt.first.y), 0) ? 1 : (tri.vt.second.y - tri.vt.first.y);
    float uvSecondSegmentHeight = utils::is_equal((tri.vt.third.y - tri.vt.second.y), 0) ? 1 : (tri.vt.third.y - tri.vt.second.y);

    float uv_diff_y = uvTotalHeight / totalHeight;
    float j = 0;
    Color col_sqr = Color(std::rand() % 255, std::rand() % 255, std::rand() % 255, 255);
    for(int i = 0; i <= totalHeight; ++i, j += uv_diff_y) {
        bool second_segment = i > firstSegmentHeight || utils::is_equal(tri.v.second.y, tri.v.first.y);
        bool uv_second_segment = j > uvFirstSegmentHeight || utils::is_equal(tri.vt.second.y, tri.vt.first.y);

        float alpha = i/totalHeight;
        float beta = (i - (second_segment ? tri.v.second.y - tri.v.first.y : 0)) /
            (second_segment ? secondSegmentHeight : firstSegmentHeight);

        float uvAlpha = j / uvTotalHeight;
        float uvBeta = (j - (uv_second_segment ? tri.vt.second.y - tri.vt.first.y : 0)) /
            (uv_second_segment ? uvSecondSegmentHeight : uvFirstSegmentHeight);

        Vec3f v = getVec(tri.v, alpha, beta, second_segment);
        Vec3f uv = getVec(tri.vt, uvAlpha, uvBeta, uv_second_segment);

        if (v.first > v.second)
            std::swap(v.first, v.second);
        if (uv.first > uv.second)
            std::swap(uv.first, uv.second);

        float diff_uv_x = uv.second - uv.first;
        float diff_v_x = v.second - v.first;
        float delta_diff = diff_v_x == 0 ? 1 : diff_uv_x / diff_v_x;

        float uvx = uv.first;
        int x = (v.first);
        int y = (v.third + i);
        for (; x <= v.second; ++x, uvx += delta_diff) {
            float depth = utils::getDepth({x, y}, tri.v, true);

            if (zBuffer[x][y] > depth)
                continue;
            zBuffer[x][y] = depth;
            diffuse = nullptr;
            auto col = diffuse ? diffuse->get(uvx, (int)(tri.vt.third.y + j)) : colors::white;
            image->set(x, y, col * (depth / 255.));
        }
    }
    return true;
}
