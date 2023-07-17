#include "Blend.h"
#include "Utilities.h"

FuncSig findBlend(GBlendMode mode, int A)
{
  if (A == 0)
  {
    switch (mode)
    {
    case GBlendMode::kClear:
    case GBlendMode::kSrc:
    case GBlendMode::kSrcIn:
    case GBlendMode::kDstIn:
    case GBlendMode::kSrcOut:
    case GBlendMode::kDstATop:
      return &kClearf;
    case GBlendMode::kDst:
    case GBlendMode::kSrcOver:
    case GBlendMode::kDstOver:
    case GBlendMode::kDstOut:
    case GBlendMode::kSrcATop:
    case GBlendMode::kXor:
      return &kDstf;
    default:
      printf("The blend mode is not reconized! ");
      return &kSrcOverf;
    }
  }
  else if (A == 255)
  {
    switch (mode)
    {
    case GBlendMode::kClear:
    case GBlendMode::kDstOut:
      return &kClearf;
    case GBlendMode::kSrc:
    case GBlendMode::kSrcOver:
      return &kSrcf;
    case GBlendMode::kDst:
    case GBlendMode::kDstIn:
      return &kDstf;
    case GBlendMode::kDstOver:
      return &kDstOverf;
    case GBlendMode::kSrcIn:
    case GBlendMode::kSrcATop:
      return &kSrcInf;
    case GBlendMode::kSrcOut:
    case GBlendMode::kXor:
      return &kSrcOutf;
    case GBlendMode::kDstATop:
      return &kDstATopf;
    default:
      printf("The blend mode is not reconized! ");
      return &kSrcOverf;
    }
  }
  else
  {
    switch (mode)
    {
    case GBlendMode::kClear:
      return &kClearf;
    case GBlendMode::kSrc:
      return &kSrcf;
    case GBlendMode::kDst:
      return &kDstf;
    case GBlendMode::kSrcOver:
      return &kSrcOverf;
    case GBlendMode::kDstOver:
      return &kDstOverf;
    case GBlendMode::kSrcIn:
      return &kSrcInf;
    case GBlendMode::kDstIn:
      return &kDstInf;
    case GBlendMode::kSrcOut:
      return &kSrcOutf;
    case GBlendMode::kDstOut:
      return &kDstOutf;
    case GBlendMode::kSrcATop:
      return &kSrcATopf;
    case GBlendMode::kDstATop:
      return &kDstATopf;
    case GBlendMode::kXor:
      return &kXorf;
    default:
      printf("The blend mode is not reconized! ");
      return &kSrcOverf;
    }
  }
}

FuncSig findBlend(GBlendMode mode)
{
  switch (mode)
  {
  case GBlendMode::kClear:
    return &kClearf;
  case GBlendMode::kSrc:
    return &kSrcf;
  case GBlendMode::kDst:
    return &kDstf;
  case GBlendMode::kSrcOver:
    return &kSrcOverf;
  case GBlendMode::kDstOver:
    return &kDstOverf;
  case GBlendMode::kSrcIn:
    return &kSrcInf;
  case GBlendMode::kDstIn:
    return &kDstInf;
  case GBlendMode::kSrcOut:
    return &kSrcOutf;
  case GBlendMode::kDstOut:
    return &kDstOutf;
  case GBlendMode::kSrcATop:
    return &kSrcATopf;
  case GBlendMode::kDstATop:
    return &kDstATopf;
  case GBlendMode::kXor:
    return &kXorf;
  default:
    printf("The blend mode is not reconized! ");
    return &kSrcOverf;
  }
}
GPixel kClearf(GPixel src, GPixel dst)
{
  return GPixel_PackARGB(0, 0, 0, 0);
}

GPixel kSrcf(GPixel src, GPixel dst)
{
  return src;
}

GPixel kDstf(GPixel src, GPixel dst)
{
  return dst;
}

GPixel kSrcOverf(GPixel src, GPixel dst)
{
  int Da = GPixel_GetA(dst);
  if (Da == 0)
  {
    return src;
  }
  else
  {
    int Sa = GPixel_GetA(src);
    int A = Sa + div255(Da * (255 - Sa));
    int R = GPixel_GetR(src) + div255(GPixel_GetR(dst) * (255 - Sa));
    int G = GPixel_GetG(src) + div255(GPixel_GetG(dst) * (255 - Sa));
    int B = GPixel_GetB(src) + div255(GPixel_GetB(dst) * (255 - Sa));
    return GPixel_PackARGB(A, R, G, B);
  }
}

GPixel kDstOverf(GPixel src, GPixel dst)
{
  int Da = GPixel_GetA(dst);
  if (Da == 0)
  {
    return src;
  }
  else if (Da == 255)
  {
    return dst;
  }
  else
  {
    int A = Da + div255(GPixel_GetA(src) * (255 - Da));
    int R = GPixel_GetR(dst) + div255(GPixel_GetR(src) * (255 - Da));
    int G = GPixel_GetG(dst) + div255(GPixel_GetG(src) * (255 - Da));
    int B = GPixel_GetB(dst) + div255(GPixel_GetB(src) * (255 - Da));
    return GPixel_PackARGB(A, R, G, B);
  }
}

GPixel kSrcInf(GPixel src, GPixel dst)
{
  int Da = GPixel_GetA(dst);
  if (Da == 0)
  {
    return GPixel_PackARGB(0, 0, 0, 0);
  }
  else if (Da == 255)
  {
    return src;
  }
  else
  {
    int A = div255(Da * GPixel_GetA(src));
    int R = div255(Da * GPixel_GetR(src));
    int G = div255(Da * GPixel_GetG(src));
    int B = div255(Da * GPixel_GetB(src));
    return GPixel_PackARGB(A, R, G, B);
  }
}

GPixel kDstInf(GPixel src, GPixel dst)
{
  int Da = GPixel_GetA(dst);
  if (Da == 0)
  {
    return GPixel_PackARGB(0, 0, 0, 0);
  }
  else
  {
    int Sa = GPixel_GetA(src);
    int A = div255(Sa * Da);
    int R = div255(Sa * GPixel_GetR(dst));
    int G = div255(Sa * GPixel_GetG(dst));
    int B = div255(Sa * GPixel_GetB(dst));
    return GPixel_PackARGB(A, R, G, B);
  }
}

GPixel kSrcOutf(GPixel src, GPixel dst)
{
  int Da = GPixel_GetA(dst);
  if (Da == 0)
  {
    return src;
  }
  else if (Da == 255)
  {
    return GPixel_PackARGB(0, 0, 0, 0);
  }
  else
  {
    int A = div255(GPixel_GetA(src) * (255 - Da));
    int R = div255(GPixel_GetR(src) * (255 - Da));
    int G = div255(GPixel_GetG(src) * (255 - Da));
    int B = div255(GPixel_GetB(src) * (255 - Da));
    return GPixel_PackARGB(A, R, G, B);
  }
}

GPixel kDstOutf(GPixel src, GPixel dst)
{
  int Da = GPixel_GetA(dst);
  if (Da == 0)
  {
    return GPixel_PackARGB(0, 0, 0, 0);
  }
  else
  {
    int Sa = GPixel_GetA(src);
    int A = div255(Da * (255 - Sa));
    int R = div255(GPixel_GetR(dst) * (255 - Sa));
    int G = div255(GPixel_GetG(dst) * (255 - Sa));
    int B = div255(GPixel_GetB(dst) * (255 - Sa));
    return GPixel_PackARGB(A, R, G, B);
  }
}

GPixel kSrcATopf(GPixel src, GPixel dst)
{
  int Da = GPixel_GetA(dst);
  if (Da == 0)
  {
    return GPixel_PackARGB(0, 0, 0, 0);
  }
  else
  {
    int Sa = GPixel_GetA(src);
    int A = Da;
    int R = div255(Da * GPixel_GetR(src)) + div255(GPixel_GetR(dst) * (255 - Sa));
    int G = div255(Da * GPixel_GetG(src)) + div255(GPixel_GetG(dst) * (255 - Sa));
    int B = div255(Da * GPixel_GetB(src)) + div255(GPixel_GetB(dst) * (255 - Sa));
    return GPixel_PackARGB(A, R, G, B);
  }
}

GPixel kDstATopf(GPixel src, GPixel dst)
{
  int Da = GPixel_GetA(dst);
  if (Da == 0)
  {
    return src;
  }
  else if (Da == 255)
  {
    return kDstInf0(src, dst);
  }
  else
  {
    int Sa = GPixel_GetA(src);
    int A = Sa;
    int R = div255(Sa * GPixel_GetR(dst)) + div255(GPixel_GetR(src) * (255 - Da));
    int G = div255(Sa * GPixel_GetG(dst)) + div255(GPixel_GetG(src) * (255 - Da));
    int B = div255(Sa * GPixel_GetB(dst)) + div255(GPixel_GetB(src) * (255 - Da));
    return GPixel_PackARGB(A, R, G, B);
  }
}

GPixel kXorf(GPixel src, GPixel dst)
{
  int Da = GPixel_GetA(dst);
  if (Da == 0)
  {
    return src;
  }
  else if (Da == 255)
  {
    return kDstOutf0(src, dst);
  }
  else
  {
    int Sa = GPixel_GetA(src);
    int A = Sa + Da - 2 * div255(Sa * Da);
    int R = div255((255 - Sa) * GPixel_GetR(dst)) + div255((255 - Da) * GPixel_GetR(src));
    int G = div255((255 - Sa) * GPixel_GetG(dst)) + div255((255 - Da) * GPixel_GetG(src));
    int B = div255((255 - Sa) * GPixel_GetB(dst)) + div255((255 - Da) * GPixel_GetB(src));
    return GPixel_PackARGB(A, R, G, B);
  }
}

GPixel kDstInf0(GPixel src, GPixel dst)
{
  int Sa = GPixel_GetA(src);
  int A = div255(Sa * GPixel_GetA(dst));
  int R = div255(Sa * GPixel_GetR(dst));
  int G = div255(Sa * GPixel_GetG(dst));
  int B = div255(Sa * GPixel_GetB(dst));
  return GPixel_PackARGB(A, R, G, B);
}

GPixel kDstOutf0(GPixel src, GPixel dst)
{
  int Sa = GPixel_GetA(src);
  int A = div255(GPixel_GetA(dst) * (255 - Sa));
  int R = div255(GPixel_GetR(dst) * (255 - Sa));
  int G = div255(GPixel_GetG(dst) * (255 - Sa));
  int B = div255(GPixel_GetB(dst) * (255 - Sa));
  return GPixel_PackARGB(A, R, G, B);
}