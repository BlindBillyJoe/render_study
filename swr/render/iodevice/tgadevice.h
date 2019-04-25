#ifndef TGADEVICE_H
#define TGADEVICE_H

#include "tgaimage.h"
#include "iodevice.h"

class Color;

class TgaDevice : public IODevice, public TGAImage
{
public:
    TgaDevice();
	TgaDevice(int w, int h, int bpp);
	TgaDevice(const TgaDevice &img);

    ~TgaDevice() override;

    void begin() override;
    void set(int x, int y, const Color& color) override;
    void end() override;

    Color get(int x, int y) const override;

    int width() const override;
    int height() const override;
};

#endif //TGADEVICE_H
