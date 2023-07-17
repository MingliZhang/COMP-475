#include <vector>
#include "GPixel.h"
#include "GColor.h"
#include "GRect.h"
#include "GBitmap.h"
#include "GBlendMode.h"
#include "GPoint.h"
#include "GPaint.h"
#include "Edge.h"


// Input: a Gcolor values
// Output: a GPixel values
// Function: takes a Gcolor varaible and map it to a GPixel variable.
// It will first pin the color to be between 0-1, then multiply rgb by a, and finally map to 0-255.
GPixel color2pixel(GColor color);

// Input: A floating point GRect
// Output: An GIRect
// Function: Rounds the floating point value to an int value so that it can be made into a GIRect.
GIRect roundSize(GRect rect);

// Input: An GRect value and a GBitmap value
// Output: A Boolean value
// Function: If the two GRect intersects or not.
// The first argument is updated to the intersection GRect.
bool clipRect(GRect &rect, GRect bounds);

// input: A number, integer.
// output: The input value divided by 255.
// Divides a number by 255. This is an approximation
unsigned div255(unsigned value);

bool compairEdges(Edge e0, Edge e1);

void clip(GPoint p0, GPoint p1, GRect bounds, std::vector<Edge> &edgeList);

bool compairPixels(GPixel p0, GPixel p1);