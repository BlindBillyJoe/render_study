#include "tgaimage.h"
#include "renderer.h"
#include "objparser.h"
#include "model.h"

void render()
{
    TGAImage image(1024, 1024, TGAImage::RGBA);
    TGAImage diffuse(1024, 1024, TGAImage::RGBA);
    diffuse.read_tga_file("c:/projects/ge/african_head_diffuse.tga");
    diffuse.flip_vertically();
    Renderer renderer;
    ObjParser parser;
    auto ptr = parser.parse("c:/projects/ge/african_head.obj");
    renderer.render(ptr, &image, &diffuse);
    image.flip_vertically();
    image.write_tga_file("c:/projects/ge/result.tga");
    delete ptr;
}

void testRender()
{
    TGAImage out(1024, 1024, TGAImage::RGBA);
    Renderer render;
    render.triangle(
        {
            { 370, 312 },
            { 343, 333 },
            { 349, 353 }
        },
        &out,
        TGAColor(255, 0, 0, 255)
    );
    render.triangle(
        {
            { 761, 994 },
            { 756, 995 },
            { 757, 1000 }
        },
        &out,
        TGAColor(0, 255, 0, 255)
    );
    out.write_tga_file("c:/projects/ge/triangles.tga");
}

void testDiffuse()
{
    TGAImage out(1024, 1024, TGAImage::RGBA);
    TGAImage diffuse(1024, 1024, TGAImage::RGBA);
    diffuse.read_tga_file("c:/projects/ge/african_head_diffuse.tga");
    const int w = diffuse.get_width();
    const int h = diffuse.get_height();
    Renderer render;
    ObjParser parser;
    auto ptr = parser.parse("c:/projects/ge/african_head.obj");
    for (size_t i = 0; i != ptr->nfaces(); ++i) {
        const Veci& f = ptr->face(i);
        render.triangle(
            {
                { (ptr->uv(f[0].second).x) * w, (ptr->uv(f[0].second).y) * h },
                { (ptr->uv(f[1].second).x) * w, (ptr->uv(f[1].second).y) * h },
                { (ptr->uv(f[2].second).x) * w, (ptr->uv(f[2].second).y) * h },
            },
            &out,
            TGAColor(std::rand() % 255, std::rand() % 255, std::rand() % 255, 255)
        );
    }
    out.flip_vertically();
    out.write_tga_file("c:/projects/ge/diffuse_test.tga");
}

int main()
{
    // testRender();
    render();
    // testDiffuse();
    return 0;
}
