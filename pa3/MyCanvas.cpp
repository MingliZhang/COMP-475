/*
 *  Copyright 2021 <me>
 */
#include <cmath>
#include <vector>
#include <stack>

#include "GCanvas.h"
#include "GBitmap.h"
#include "GColor.h"
#include "GRect.h"
#include "GPaint.h"
#include "GMath.h"
#include "GMatrix.h"
#include "GShader.h"

#include "Utilities.h"
#include "Edge.h"
#include "Blend.h"

class MyCanvas : public GCanvas
{
  // fDevice is the field inside the canvase that
  // GPixel* p = fDevice.getAddr(x, y)
public:
  MyCanvas(const GBitmap &device) : fDevice(device)
  {
    GMatrix initial = GMatrix();
    CTMStack.push(initial);
  }

  void save() override
  {
    GMatrix copy = GMatrix(currentM[0], currentM[1], currentM[2], currentM[3], currentM[4], currentM[5]);
    CTMStack.push(copy);
  };
  void restore() override
  {
    if (CTMStack.size() <= 1)
    {
      printf("No more history to restore from!");
      return;
    }
    else
    {
      currentM = CTMStack.top();
      CTMStack.pop();
    }
  };
  void concat(const GMatrix &matrix) override
  {
    currentM.preConcat(matrix);
  };
  /**
     *  Fill the entire canvas with the specified color, using the specified blendmode.
     */
  void drawPaint(const GPaint &paint) override
  {
    // covert the color into a Gpixel,
    // write this pixel to the whole image.
    GPoint quad[4];
    quad[0] = {0, 0};
    quad[1] = {(float)fDevice.width(), 0};
    quad[2] = {(float)fDevice.width(), (float)fDevice.height()};
    quad[3] = {0, (float)fDevice.height()};
    this->drawConvexPolygon(quad, 4, paint);
  }

  /**
     *  Fill the rectangle with the color, using the specified blendmode.
     *
     *  The affected pixels are those whose centers are "contained" inside the rectangle:
     *      e.g. contained == center > min_edge && center <= max_edge
     */
  void drawRect(const GRect &rect, const GPaint &paint) override
  {
    GPoint quad[4];
    quad[0] = {rect.left(), rect.top()};
    quad[1] = {rect.right(), rect.top()};
    quad[2] = {rect.right(), rect.bottom()};
    quad[3] = {rect.left(), rect.bottom()};
    this->drawConvexPolygon(quad, 4, paint);
  }
  /**
     *  Fill the convex polygon with the color and blendmode,
     *  following the same "containment" rule as rectangles.
     */
  void drawConvexPolygon(const GPoint ipoints[], int count, const GPaint &paint) override
  {
    GRect bounds = GRect::MakeWH(fDevice.width(), fDevice.height());
    GPoint points[count];
    currentM.mapPoints(points, ipoints, count);
    for (int i = 0; i < count; i++)
    {
      GPoint p0 = points[i];
      GPoint p1 = points[(i + 1) % count];
      clip(p0, p1, bounds, edgeList);
    }
    std::sort(edgeList.begin(), edgeList.end(), compairEdges);

    if (edgeList.size() < 2)
    {
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
    for (int i = topBound; i < bottomBound; i++)
    {
      GRect rect = GRect::MakeLTRB(leftX, i, rightX, i + 1);
      blit(leftX, rightX, i, paint);

      // Check if an edge is finished or not.
      if (i >= leftEdge.bottom)
      {
        leftEdge = edgeList.front();
        edgeList.erase(edgeList.begin());
        leftX = leftEdge.x0;
      }
      if (i >= rightEdge.bottom)
      {
        rightEdge = edgeList.front();
        edgeList.erase(edgeList.begin());
        rightX = rightEdge.x0;
      }
      if (leftX > rightX)
      {
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
  GMatrix currentM;
  std::stack<GMatrix> CTMStack;
  GPixel dstH;
  GPixel resultH;
  GPixel srcH;

  void blit(float leftf, float rightf, float yf, GPaint paint)
  {
    GShader *shader = paint.getShader();
    GPixel zeroPixel = GPixel_PackARGB(0, 0, 0, 0);
    int left = fmax(GRoundToInt(leftf), 0);
    int right = fmin(GRoundToInt(rightf), fDevice.width());
    int y = GRoundToInt(yf);
    if (shader == nullptr)
    {
      GPixel src = color2pixel(paint.getColor());
      FuncSig blend = findBlend(paint.getBlendMode(), GPixel_GetA(src));
      for (int x = left; x < right; x++)
      {
        GPixel *address = fDevice.getAddr(x, y);
        if (blend == kClearf)
        {
          *address = zeroPixel;
        }
        else if (blend == kSrcf)
        {
          *address = src;
        }
        else if (blend != kDstf)
        {
          // GPixel dstH;
          // GPixel resultH;
          GPixel dst = *address;
          if (dst==dstH && src == srcH)
          // if (dst == dstH)
          {
            *address = resultH;
          }
          else
          {
            dstH = dst;
            srcH = src;
            resultH = blend(src, dst);
            *address = resultH;
          }
        }
      }
    }
    else
    {
      if (!shader->setContext(currentM))
      {
        return;
      }
      int width = right - left;
      GPixel src[width];
      if (shader->isOpaque())
      {
        FuncSig blend = findBlend(paint.getBlendMode(), 255);
        if (blend != kClearf || blend != kDstf)
        {
          shader->shadeRow(left, y, width, src);
        }
        for (int x = left; x < right; x++)
        {
          GPixel *address = fDevice.getAddr(x, y);
          if (blend == kClearf)
          {
            *address = zeroPixel;
          }
          else if (blend == kSrcf)
          {
            *address = src[x - left];
          }
          else if (blend != kDstf)
          {
            *address = blend(src[x - left], *address);
          }
        }
      }
      else
      {
        FuncSig blend = findBlend(paint.getBlendMode());
        if (blend != kClearf || blend != kDstf)
        {
          shader->shadeRow(left, y, width, src);
        }
        
        for (int x = left; x < right; x++)
        {
          GPixel *address = fDevice.getAddr(x, y);
          if (blend == kClearf)
          {
            *address = zeroPixel;
          }
          else if (blend == kSrcf)
          {
            *address = src[x - left];
          }
          else if (blend != kDstf)
          {
            *address = blend(src[x - left], *address);
          }
        }
      }
    }
  }
};

std::unique_ptr<GCanvas> GCreateCanvas(const GBitmap &device)
{
  return std::unique_ptr<GCanvas>(new MyCanvas(device));
}

std::string GDrawSomething(GCanvas *canvas, GISize dim)
{
  GBitmap spock;
  spock.readFromFile("apps/spock.png");
  int width = dim.width();
  int height = dim.height();
  int spock_w = spock.width();
  int spock_h = spock.height();
  GPoint pts[] = {{0, 0}, {0, height}, {width, height}, {width, 0}};
  std::unique_ptr<GShader> shader = GCreateBitmapShader(spock, GMatrix::Scale((float)width / (float)spock_w, (float)height / (float)spock_h));
  GPaint paint(shader.get());
  int loops = 50;
  canvas->drawConvexPolygon(pts, 4, paint);
  for (int i = 0; i < loops; i ++){
    canvas->scale(1 - 1. / loops, 1 - 1. / loops);
    canvas->drawConvexPolygon(pts, 4, paint);
  }
  return "Spoooooock";
}
