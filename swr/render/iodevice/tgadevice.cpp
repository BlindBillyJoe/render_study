#include "tgadevice.h"
#include "../../utils/color.h"

namespace
{
    Color tgaColor2swrColor(const TGAColor& col)
    {
        return Color(col.r, col.g, col.b, col.a);
    }
}

TgaDevice::TgaDevice()
    : IODevice()
    , TGAImage()
{

}

TgaDevice::TgaDevice(int w, int h, int bpp)
    : IODevice()
    , TGAImage(w, h, bpp)
{

}

TgaDevice::TgaDevice(const TgaDevice& other)
    : IODevice()
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

Color TgaDevice::get(int x, int y) const
{
    return std::move(tgaColor2swrColor(TGAImage::get(x, y)));
}

void TgaDevice::end()
{
    
}

int TgaDevice::width() const
{
    return get_width();
}

int TgaDevice::height() const
{
    return get_height();
}