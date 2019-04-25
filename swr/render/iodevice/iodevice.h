#ifndef IODEVICE_H
#define IODEVICE_H

#include "../../utils/color.h"

class IODevice
{
public:
    virtual ~IODevice() { }

    virtual void begin() = 0;
    virtual void set(int x, int y, const Color& color) = 0;
    virtual void end() = 0;

    virtual Color get(int x, int y) const = 0;

    virtual int width() const = 0;
    virtual int height() const = 0;
};

#endif //IODEVICE_H
