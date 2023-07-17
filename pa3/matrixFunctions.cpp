#include <cmath>

#include "GMatrix.h"
#include "GPoint.h"

/** Enums naming the 6 elements
     *
     *  [ SX  KX  TX ]
     *  [ KY  SY  TY ]
     *  [  0   0   1 ]
     *
     *  'S' - Scale
     *  'K' - sKew
     *  'T' - Translate
     */

GMatrix::GMatrix(){
  this->fMat[0] = 1;
  this->fMat[1] = 0;
  this->fMat[2] = 0;
  this->fMat[3] = 0;
  this->fMat[4] = 1;
  this->fMat[5] = 0;
}
GMatrix GMatrix::Translate(float tx, float ty)
{
  return GMatrix(1, 0, tx, 0, 1, ty);
}
GMatrix GMatrix::Scale(float sx, float sy)
{
  return GMatrix(sx, 0, 0, 0, sy, 0);
}
GMatrix GMatrix::Rotate(float radians)
{
  return GMatrix(cos(radians), -sin(radians), 0, sin(radians), cos(radians),0);
}
GMatrix GMatrix::Concat(const GMatrix &a, const GMatrix &b)
{
  float sxa = a[0];  // a
  float kxa = a[1];  // b
  float txa = a[2];  // c
  float kya = a[3];  // d
  float sya = a[4];  // e
  float tya = a[5];  // f

  float sxb = b[0];  // g
  float kxb = b[1];  // h
  float txb = b[2];  // i
  float kyb = b[3];  // j
  float syb = b[4];  // k
  float tyb = b[5];  // l
  return GMatrix(sxa*sxb+kxa*kyb, sxa*kxb+kxa*syb, sxa*txb+kxa*tyb+txa, kya*sxb+sya*kyb, kya*kxb+sya*syb, kya*txb+sya*tyb+tya);
}

// [ a  b  c ] [ x ]
// [ d  e  f ] [ y ]
// [ 0  0  1 ] [ 1 ]
bool GMatrix::invert(GMatrix *inverse) const
{
  float a = this->fMat[0];
  float b = this->fMat[1];
  float c = this->fMat[2];
  float d = this->fMat[3];
  float e = this->fMat[4];
  float f = this->fMat[5];

  // printf("First: %f, %f, %f, %f, %f, %f.\n", a, b, c, d, e, f);
  // printf("Second: %f, %f, %f, %f, %f, %f.\n", this->fMat[GMatrix::SX], this->fMat[GMatrix::KX], this->fMat[GMatrix::TX], this->fMat[GMatrix::KY], this->fMat[GMatrix::SY], this->fMat[GMatrix::TY]);

  float det = a*e - b*d;
  if (det == 0){
    return false;
  }
  det = 1 / det;

  inverse -> fMat[0] = det * e;
  inverse -> fMat[1] = det * -b;
  inverse -> fMat[2] = det * (b*f-e*c);
  inverse -> fMat[3] = det * -d;
  inverse -> fMat[4] = det * a;
  inverse -> fMat[5] = det * -(a*f-d*c);
  return true;
}

    /**
     *  Transform the set of points in src, storing the resulting points in dst, by applying this
     *  matrix. It is the caller's responsibility to allocate dst to be at least as large as src.
     *
     *  [ a  b  c ] [ x ]     x' = ax + by + c
     *  [ d  e  f ] [ y ]     y' = dx + ey + f
     *  [ 0  0  1 ] [ 1 ]
     *
     *  Note: It is legal for src and dst to point to the same memory (however, they may not
     *  partially overlap). Thus the following is supported.
     *
     *  GPoint pts[] = { ... };
     *  matrix.mapPoints(pts, pts, count);
     */
void GMatrix::mapPoints(GPoint dst[], const GPoint src[], int count) const
{
  float a = this->fMat[0];
  float b = this->fMat[1];
  float c = this->fMat[2];
  float d = this->fMat[3];
  float e = this->fMat[4];
  float f = this->fMat[5];

  // printf("First: %f, %f, %f, %f, %f, %f.\n", a, b, c, d, e, f);
  // printf("Second: %f, %f, %f, %f, %f, %f.\n", this->fMat[GMatrix::SX], this->fMat[GMatrix::KX], this->fMat[GMatrix::TX], this->fMat[GMatrix::KY], this->fMat[GMatrix::SY], this->fMat[GMatrix::TY]);
  for (int i = 0; i < count; i ++){
    float x = src[i].x();
    float y = src[i].y();
    dst[i] = GPoint::Make(a*x + b*y + c, d*x+e*y+f);
    // dst[i] = GPoint::Make(SX*x + KX*y + TX, KY*x+SY*y+TY);
  }
}