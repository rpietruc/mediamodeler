#ifndef MEDIA_SPECTRUMFRAME_H
#define MEDIA_SPECTRUMFRAME_H

#include "matrixframe.h"

namespace media {

class SpectrumFrame : public MatrixFrame
    {
public:
    enum
        {
        Time = SampleNo, // 0
        Frequency = VectorNo, // 1
        Dimensions
        };

    explicit SpectrumFrame();

    inline void setFrameSamples(int aSampleNo) { setMaxSamples(aSampleNo); }
    void setSampleRate(int aSampleRate);
    inline void setTimeStamp(double aTimeStamp) { mDimensions[Time].mStartLocation = aTimeStamp; }
    inline void incrementTimeStamp() { mDimensions[Time].mStartLocation += mDimensions[Time].mDelta * mDimensions[Time].mResolution; }

    void setResolution(int aResolution, qreal aMaxFrequency);
    int getResolution() const { return mDimensions[VectorNo].mResolution; }

    void operator+=(const FrameBase &aFrame);
    };

} // namespace media

#endif // MEDIA_SPECTRUMFRAME_H
