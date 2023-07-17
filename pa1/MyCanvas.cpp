/*
 *  Copyright 2021 <me>
 */
#include <cmath>

#include "GCanvas.h"
#include "GBitmap.h"
#include "Gcolor.h"
#include "GRect.h"
#include "Utilities.h"
#include "GMath.h"

class MyCanvas : public GCanvas
{
    // fDevice is the field inside the canvase that
    // GPixel* p = fDevice.getAddr(x, y)
public:
    MyCanvas(const GBitmap &device) : fDevice(device) {}

    void clear(const GColor &color) override
    {
        // covert the color into a Gpixel,
        // write this pixel to the whole image.
        GPixel src = color2pixel(color);
        for (int y = 0; y < fDevice.height(); y++)
        {
            for (int x = 0; x < fDevice.width(); x++)
            {
                GPixel *address = fDevice.getAddr(x, y);
                *address = src;
            }
        }
    }

    void fillRect(const GRect &rect, const GColor &color) override
    {
        GRect bounds = GRect::MakeWH(fDevice.width(), fDevice.height());
        GRect temp = GRect::MakeLTRB(rect.left(), rect.top(), rect.right(), rect.bottom());
        bool intersect = clip(temp, bounds);
        if (!intersect)
            return;
        GIRect newRect = roundSize(temp);

        GPixel src = color2pixel(color);

        for (int y = newRect.top(); y < newRect.bottom(); y++)
        {
            for (int x = newRect.left(); x < newRect.right(); x++)
            {
                GPixel *address = fDevice.getAddr(x, y);
                *address = srcover(src, *address);
            }
        }
    }

private:
    // Note: we store a copy of the bitmap
    const GBitmap fDevice;
};

std::unique_ptr<GCanvas> GCreateCanvas(const GBitmap &device)
{
    return std::unique_ptr<GCanvas>(new MyCanvas(device));
}

std::string GDrawSomething(GCanvas *canvas)
{
    for (int r = 0; r <255; r ++){
        for(int a = 0; a < 255; a++){
            GColor C = {(double)r/255,(double)r/255,0,(double)a/255};
            GRect R = GRect::MakeLTRB(r,a,r+1,a+1);
            canvas->fillRect(R,C);
        }
    }
    // GColor C = {(double)50 / 255, 0, 0, (double)200/ 255};
    // GRect R = GRect::MakeLTRB(0,0, 100, 100);
    // canvas->fillRect(R, C);
    // C = {0, (double)50 / 255, 0, (double)200 / 255};
    // R = GRect::MakeLTRB(50,50,200,200);
    // canvas->fillRect(R, C);
     return "50+ shades of yellow";
}