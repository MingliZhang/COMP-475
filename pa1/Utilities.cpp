#include <cmath>
#include <iostream>

#include "Utilities.h"
#include "GPixel.h"
#include "GColor.h"
#include "GRect.h"
#include "GBitmap.h"

GPixel color2pixel(GColor color)
{
    int r = floor(GPinToUnit(color.r) * GPinToUnit(color.a) * 255 + 0.5);
    int g = floor(GPinToUnit(color.g) * GPinToUnit(color.a) * 255 + 0.5);
    int b = floor(GPinToUnit(color.b) * GPinToUnit(color.a) * 255 + 0.5);
    int a = floor(GPinToUnit(color.a) * 255 + 0.5);

    return GPixel_PackARGB(a, r, g, b);
}

GIRect roundSize(GRect rect)
{
    int L = GRoundToInt(rect.left());
    int T= GRoundToInt(rect.top());
    int R= GRoundToInt(rect.right()) ;
    int B= GRoundToInt(rect.bottom()) ;
    return GIRect::MakeLTRB(L,T,R,B);
}

bool clip(GRect &rect, GRect bounds)
{
    return rect.intersect(bounds);
};

GPixel srcover(GPixel src, GPixel dst)
{
    int A = GPixel_GetA(src) + div255(GPixel_GetA(dst) * (255 - GPixel_GetA(src)));
    int R = GPixel_GetR(src) + div255(GPixel_GetR(dst) * (255 - GPixel_GetA(src)));
    int G = GPixel_GetG(src) + div255(GPixel_GetG(dst) * (255 - GPixel_GetA(src)));
    int B = GPixel_GetB(src) + div255(GPixel_GetB(dst) * (255 - GPixel_GetA(src)));
    return GPixel_PackARGB(A, R, G, B);
}

unsigned div255(unsigned value)
{
    return (value * 65793 + (1 << 23)) >> 24;
}