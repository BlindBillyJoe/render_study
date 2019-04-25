#include "swr/render/iodevice/tgaimage.h"
#include "swr/render/renderer.h"
#include "swr/parser/objparser.h"
#include "swr/model/model.h"
#include "swr/render/iodevice/tgadevice.h"
#include "swr/utils/utils.h"

#include <memory>

void render()
{
    TgaDevice device_triangle(768, 1024, TGAImage::RGBA);
    TgaDevice diffuse(1024, 1024, TGAImage::RGBA);
    diffuse.read_tga_file("c:/projects/ge/african_head_diffuse.tga");
    diffuse.flip_vertically();
    Renderer renderer;
    ObjParser parser;
    auto ptr = parser.parse("c:/projects/ge/african_head.obj");
    renderer.render(&(*ptr), &device_triangle, &diffuse);
    device_triangle.flip_vertically();
    device_triangle.write_tga_file("c:/projects/ge/result_trianlge_raster.tga");
}

int main(int argc, char** args)
{
    render();
    return 0;
}
