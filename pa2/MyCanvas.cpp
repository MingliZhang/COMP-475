/*
 *  Copyright 2021 <me>
 */
#include <cmath>
#include <vector>

#include "GCanvas.h"
#include "GBitmap.h"
#include "GColor.h"
#include "GRect.h"
#include "GPaint.h"
#include "Utilities.h"
#include "GMath.h"
#include "Edge.h"
#include "Blend.h"

class MyCanvas : public GCanvas
{
    // fDevice is the field inside the canvase that
    // GPixel* p = fDevice.getAddr(x, y)
public:
    MyCanvas(const GBitmap &device) : fDevice(device) {}
    /**
     *  Fill the entire canvas with the specified color, using the specified blendmode.
     */
    void drawPaint(const GPaint& paint) override
    {
        // covert the color into a Gpixel,
        // write this pixel to the whole image.
        GRect rect = GRect::MakeWH(fDevice.width(), fDevice.height());
        drawRect(rect, paint);
    }

    /**
     *  Fill the rectangle with the color, using the specified blendmode.
     *
     *  The affected pixels are those whose centers are "contained" inside the rectangle:
     *      e.g. contained == center > min_edge && center <= max_edge
     */
    void drawRect(const GRect &rect, const GPaint &paint) override
    {
        GRect bounds = GRect::MakeWH(fDevice.width(), fDevice.height());
        GRect temp = GRect::MakeLTRB(rect.left(), rect.top(), rect.right(), rect.bottom());
        bool intersect = clipRect(temp, bounds);
        if (!intersect){
            return;}
        GIRect newRect = roundSize(temp);

        GPixel src = color2pixel(paint.getColor());
        GPixel zeroPixel = GPixel_PackARGB(0, 0, 0, 0);
        FuncSig blend = findBlend(paint.getBlendMode(),GPixel_GetA(src));
        for (int y = newRect.top(); y < newRect.bottom(); y++)
        {
            for (int x = newRect.left(); x < newRect.right(); x++)
            {
              GPixel *address = fDevice.getAddr(x, y);
              if (blend == kClearf){
                *address = zeroPixel;
              } else if(blend == kSrcf){
                *address = src;
              }else if (blend != kDstf){
                *address = blend(src, *address);
              }
            }
        }
    }
    /**
     *  Fill the convex polygon with the color and blendmode,
     *  following the same "containment" rule as rectangles.
     */
     // drawRect(const GRect &rect, const GPaint &paint)
    void drawConvexPolygon(const GPoint points[], int count, const GPaint &paint) override{
      GRect bounds = GRect::MakeWH(fDevice.width(), fDevice.height());
      for (int i = 0; i < count; i ++){
        GPoint p0 = points[i];
        GPoint p1 = points[(i + 1) % count];
        clip(p0, p1, bounds, edgeList);
      }
      std::sort(edgeList.begin(), edgeList.end(), compairEdges);
      
      if(edgeList.size() < 2){
        return;
      }
      int topBound = edgeList.front().top;
      int bottomBound = edgeList.back().bottom;
      Edge leftEdge = edgeList.front();
      edgeList.erase(edgeList.begin());
      Edge rightEdge = edgeList.front();
      edgeList.erase(edgeList.begin());
      
      float leftX = leftEdge.x0;
      float rightX = rightEdge.x0;
       // something here
      for (int i = topBound; i < bottomBound; i ++){
        GRect rect = GRect::MakeLTRB(leftX, i, rightX, i + 1);
        drawRect(rect, paint);
        // Check if an edge is finished or not.
        if(i>=leftEdge.bottom){
          leftEdge = edgeList.front();
          edgeList.erase(edgeList.begin());
          leftX = leftEdge.x0;
        }
        if(i>=rightEdge.bottom){
          rightEdge = edgeList.front();
          edgeList.erase(edgeList.begin());
          rightX = rightEdge.x0;
        }
        if(leftX > rightX){
          std::swap(leftEdge, rightEdge);
          std::swap(leftX, rightX);
        }
        leftX += leftEdge.slope;
        rightX += rightEdge.slope;
      }
    };

private:
    // Note: we store a copy of the bitmap
    const GBitmap fDevice;
    std::vector<Edge> edgeList;
};

std::unique_ptr<GCanvas> GCreateCanvas(const GBitmap &device)
{
    return std::unique_ptr<GCanvas>(new MyCanvas(device));
}

std::string GDrawSomething(GCanvas* canvas, GISize dim)
{
  int width = dim.width();
  int height = dim.height();

  int num = rand() % 50 + 5;
  for (int i = 0; i < num; i ++){
    GPoint p0 = GPoint::Make(rand() % width, rand() % height);
    GPoint p1 = GPoint::Make(rand() % width, rand() % height);
    GPoint p2 = GPoint::Make(rand() % width, rand() % height);
    GPoint pointArray[3] = {p0, p1, p2};
    GBlendMode mode = static_cast<GBlendMode>(rand() % 12);
    GPaint paint;
    paint.setRGBA(((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)),((double) rand() / (RAND_MAX)));
    paint.setBlendMode(mode);
    canvas->drawConvexPolygon(pointArray, 3, paint);
  }
  return "Random number of random triangles with random color using random blend mode.";
}
