#ifndef PAINTDEVICE_H
#define PAINTDEVICE_H

class Color;
class PaintDevice
{
public:
    virtual ~PaintDevice()
    {

    }
    virtual void begin() = 0;
    virtual void set(int x, int y, const Color& color) = 0;
    virtual void end() = 0;
};

#endif //PAINTDEVICE_H
