#ifndef TGADEVICE_H
#define TGADEVICE_H

#include "tgaimage.h"
#include "paintdevice.h"

class Color;

class TgaDevice : public PaintDevice, public TGAImage
{
public:
    TgaDevice();
    ~TgaDevice() override;
	TgaDevice(int w, int h, int bpp);
	TgaDevice(const TgaDevice &img);

    void begin() override;
    void set(int x, int y, const Color& color) override;
    void end() override;
};

#endif //TGADEVICE_H
