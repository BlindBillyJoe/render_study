#include <iostream>

#include "tgaimage.h"
#include "renderer.h"
#include "objparser.h"
#include "model.h"

int main()
{
	TGAImage image(926, 926, TGAImage::RGBA);
	TGAImage diffuse(926, 926, TGAImage::RGBA);
	diffuse.read_tga_file("african_head_diffuse.tga");
	diffuse.flip_vertically();
	Renderer renderer;
	auto height = image.get_height();
	auto width = image.get_width();
	ObjParser parser;
	auto ptr = parser.parse("african_head.obj");
	renderer.render(ptr, &image, &diffuse);
	image.flip_vertically();
	image.write_tga_file("result.tga");
	delete ptr;
	return 0;
}