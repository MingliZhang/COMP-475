#include "GBlendMode.h"
#include "GPixel.h"

typedef GPixel (* FuncSig)(GPixel src, GPixel dst);
// Input: A GBlendMode
// Output: A pointer that points to the blend function
// Function: Finds which blend funciton should be called and uses that.
FuncSig findBlend(GBlendMode mode, int A);

FuncSig findBlend(GBlendMode mode);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: 0
GPixel kClearf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: S
GPixel kSrcf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: D
GPixel kDstf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: S + (1 - Sa)*D
GPixel kSrcOverf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: D + (1 - Da)*S
GPixel kDstOverf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: Da * S
GPixel kSrcInf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: Sa * D
GPixel kDstInf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: (1 - Da)*S
GPixel kSrcOutf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: (1 - Sa)*D
GPixel kDstOutf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: Da*S + (1 - Sa)*D
GPixel kSrcATopf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: Sa*D + (1 - Da)*S
GPixel kDstATopf(GPixel src,GPixel dst);

// Input: Two GPixel values.
// Output: A GPixel value.
// Function: (1 - Sa)*D + (1 - Da)*S
GPixel kXorf(GPixel src,GPixel dst);

// These two does exactly the same as their non-zero counter part without the Da value checker.
GPixel kDstOutf0(GPixel src, GPixel dst);
GPixel kDstInf0(GPixel src, GPixel dst);