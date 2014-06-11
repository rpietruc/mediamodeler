#ifndef MEDIA_FILTERFRAME_H
#define MEDIA_FILTERFRAME_H

#include "qubeframe.h"

namespace media {

class FilterFrame : public QubeFrame
    {
public:
    enum
        {
        Numerator = 0,
        Denominator //= 1
        };

    enum
        {
        Time = PlaneNo,
        Coefficients = VectorNo,
        Polynomial = ParamNo,
        Dimensions
        };

    explicit FilterFrame();

    inline void setFrameSamples(int aVectorNo) { setVectorSizeT(aVectorNo); }
    inline void setResolution(int aResolution) { setVectorSizeT(aResolution); }

    void setSampleRate(int aSampleRate);
    inline void setTimeStamp(double aTimeStamp) { mDimensions[Time].mStartLocation = aTimeStamp; }
    inline void incrementTimeStamp() { mDimensions[Time].mStartLocation += mDimensions[Time].mDelta * mDimensions[Time].mResolution; }
    };

} // namespace media

#endif // MEDIA_FILTERFRAME_H
