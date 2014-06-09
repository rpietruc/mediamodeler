#ifndef MEDIA_FILTERFRAME_H
#define MEDIA_FILTERFRAME_H

#include "matrixframe.h"

namespace media {

class FilterFrame : public Matrix3DFrame
    {
public:
    enum
        {
        Numerator = 0,
        Denominator //= 1
        };

    enum
        {
        Time = SampleNo,
        Coefficients = VectorNo,
        Polynomial = PlaneNo,
        Dimensions
        };

    explicit FilterFrame();

    inline void setFrameSamples(int aSampleNo) { setMaxSamplesT(aSampleNo); }
    inline void setResolution(int aResolution) { setMaxVectorsT(aResolution); }

    void setSampleRate(int aSampleRate);
    inline void setTimeStamp(double aTimeStamp) { mDimensions[Time].mStartLocation = aTimeStamp; }
    inline void incrementTimeStamp() { mDimensions[Time].mStartLocation += mDimensions[Time].mDelta * mDimensions[Time].mResolution; }
    };

} // namespace media

#endif // MEDIA_FILTERFRAME_H
