#include <cmath>

#include "GShader.h"
#include "GBitmap.h"
#include "GMatrix.h"

class BitmapShader : public GShader
{
public:
  BitmapShader(const GBitmap &bitMap, const GMatrix &localM): bitMap(bitMap), localM(localM) {}

  bool isOpaque() override{
    return bitMap.isOpaque();
  };
  bool setContext(const GMatrix &ctm) override{
    GMatrix temp = GMatrix::Concat(ctm, localM);
    return temp.invert(&inverseM);
  };
  void shadeRow(int ix, int iy, int count, GPixel row[]) override{
    float xtemp = ix + 0.5;
    float ytemp = iy + 0.5;

    float a = inverseM[0];
    float b = inverseM[1];
    float c = inverseM[2];
    float d = inverseM[3];
    float e = inverseM[4];
    float f = inverseM[5];

    float x = a * xtemp + b * ytemp + c;
    float y = d * xtemp + e * ytemp + f;
    if(d == 0){
      
      y = fmin(fmax(0, y), bitMap.height() - 1);
      int addry = GFloorToInt(y);
      for (int i = 0; i < count; i++)
      {
        x = fmin(fmax(0, x), bitMap.width() - 1);
        int addrx = GFloorToInt(x);
        row[i] = *bitMap.getAddr(addrx,addry);
        x += a;
      }
    }else{
      for (int i = 0; i < count; i++)
      {
        x = fmin(fmax(0, x), bitMap.width() - 1);
        y = fmin(fmax(0, y), bitMap.height() - 1);
        int addrx = GFloorToInt(x);
        int addry = GFloorToInt(y);
        row[i] = *bitMap.getAddr(addrx,addry);
        x += a;
        y += d;
      }
    }
  };
  private:
    GBitmap bitMap;
    GMatrix localM;
    GMatrix inverseM;
};
std::unique_ptr<GShader> GCreateBitmapShader(const GBitmap &bitMap, const GMatrix &localM){
  if (bitMap.pixels() == NULL)
  {
    return nullptr;
  }

  return std::unique_ptr<GShader>(new BitmapShader(bitMap, localM));
}