#include <cmath>
#include <iostream>
#include <vector>

#include "Utilities.h"
#include "GPixel.h"
#include "GColor.h"
#include "GRect.h"
#include "GBitmap.h"
#include "GBlendMode.h"
#include "GPoint.h"
#include "Edge.h"

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
    int T = GRoundToInt(rect.top());
    int R = GRoundToInt(rect.right());
    int B = GRoundToInt(rect.bottom());
    return GIRect::MakeLTRB(L, T, R, B);
}

bool clipRect(GRect &rect, GRect bounds)
{
    return rect.intersect(bounds);
};

unsigned div255(unsigned value)
{
    return (value * 65793 + (1 << 23)) >> 24;
}

bool compairEdges(Edge e0, Edge e1) {
    if (e0.top < e1.top) {
        return true;
    }else if (e1.top < e0.top) {
        return false;
    }
    if (e0.x0 < e1.x0) {
        return true;
    }else if (e1.x0 < e0.x0) {
        return false;
    }
    return e0.slope < e1.slope;
}

void clip(GPoint p0, GPoint p1, GRect bounds, std::vector<Edge> &edgeList){
  // Make p0 on top and p1 at bottom
  int rightBound = bounds.right();
  int leftBound = bounds.left();
  int topBound = bounds.top();
  int bottomBound = bounds.bottom();
  if (p0.y() > p1.y())
  {
    std::swap(p0, p1);
  }
  if(p0.y()>=bottomBound || p1.y() <= topBound)
  {
    return;
  }
  if(p0.y()< topBound){
    float slope = (p1.x() - p0.x()) / (p1.y() - p0.y());
    float newX = p0.x() +  (topBound - p0.y()) * slope;
    p0.set(newX, topBound);
  }
  if(p1.y()> bottomBound){
    float slope = (p1.x() - p0.x()) / (p1.y() - p0.y());
    float newX = p1.x() -  (p1.y() - bottomBound) * slope;
    p1.set(newX, bottomBound);
  }
  // Make p0 on left and p1 at rigth
  if (p0.x() > p1.x())
  {
    std::swap(p0, p1);
  }
  if (p1.x() <=leftBound){
    // Project the edge to the left bound.
    p0.fX = leftBound;
    p1.fX = leftBound;
    Edge newEdge = Edge::makeEdge(p0, p1);
    edgeList.push_back(newEdge);
    return;
  }
  if (p0.x() >= rightBound){
    // Project the edge to the right bound.
    p0.fX = rightBound;
    p1.fX = rightBound;
    Edge newEdge = Edge::makeEdge(p0, p1);
    edgeList.push_back(newEdge);
    return;
  }
  if (p1.x() > rightBound){
    float slope = (p1.y() - p0.y()) / (p1.x() - p0.x());
    float b = p0.y() - p0.x() * slope;
    GPoint midPoint = GPoint::Make(rightBound, slope * rightBound + b);
    p1.fX = rightBound;
    Edge newEdge = Edge::makeEdge(p0, midPoint);
    Edge projectedEdge = Edge::makeEdge(midPoint, p1);
    if(newEdge.useful){
    edgeList.push_back(newEdge);
    }
    edgeList.push_back(projectedEdge);
    return;
  }
  // If a part is in bounds and the rest sticks out left border.
  if(p0.x() < leftBound){
    float slope = (p1.y() - p0.y()) / (p1.x() - p0.x());
    float b = p1.y() - p1.x() * slope;
    GPoint midPoint = GPoint::Make(leftBound, slope * leftBound + b);
    p0.fX = leftBound;
    Edge newEdge = Edge::makeEdge(p1, midPoint);
    Edge projectedEdge = Edge::makeEdge(midPoint, p0);
    if(newEdge.useful){
    edgeList.push_back(newEdge);
    }
    edgeList.push_back(projectedEdge);
    return;
  }
  
  Edge newEdge = Edge::makeEdge(p0, p1);
  if(newEdge.useful){
    edgeList.push_back(newEdge);
  }
}