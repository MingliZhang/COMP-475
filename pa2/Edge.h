#ifndef Edge_DEFINED
#define Edge_DEFINED

#include "GMath.h"
#include "GPoint.h"

// x = dx/dy * y + b;
struct Edge{
    int top, bottom;
    float slope, x0;
    bool useful;

    static Edge makeEdge(GPoint p0, GPoint p1){
      // Make p0 the point on top and p1 the one on bottom
      if (p0.y() > p1.y()){
        std::swap(p0, p1);
      }
      int top = GRoundToInt(p0.y());
      int bottom = GRoundToInt(p1.y());
      if (top == bottom ){
          return {-1,-1,-1.,-1.,false};
      }
      float slope = (p1.x() - p0.x()) / (p1.y() - p0.y());
      float x0 = slope * (static_cast<float>(top)+0.5 - p0.y()) + p0.x();
      return {top, bottom, slope, x0, true};
    }
};

#endif