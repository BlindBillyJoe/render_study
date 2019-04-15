#include "tgadevice.h"
#include "color.h"

TgaDevice::TgaDevice()
    : PaintDevice()
    , TGAImage()
{

}

TgaDevice::TgaDevice(int w, int h, int bpp)
    : PaintDevice()
    , TGAImage(w, h, bpp)
{

}

TgaDevice::TgaDevice(const TgaDevice& other)
    : PaintDevice()
    , TGAImage(other)
{

}

TgaDevice::~TgaDevice()
{
    
}

void TgaDevice::begin()
{

}

void TgaDevice::set(int x, int y, const Color& color)
{
    TGAImage::set(x, y, TGAColor(color.r, color.g, color.b, color.a));
}

void TgaDevice::end()
{
    
}