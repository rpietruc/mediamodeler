#ifndef MEDIA_SPECTRUMFRAME_H
#define MEDIA_SPECTRUMFRAME_H

#include "matrixframe.h"

namespace media {

class SpectrumFrame : public MatrixFrame
    {
public:
    enum
        {
        Time = VectorNo, // 0
        Frequency = ParamNo, // 1
        Dimensions
        };

    explicit SpectrumFrame() {}
    void operator+=(const FrameBase &aFrame);

    void setFrameSamples(int aSamplesNo) { setMaxVectors(aSamplesNo); }
    void setSampleRate(int aSampleRate);
    void setTimeStamp(double aTimeStamp) { mDimensions[Time].mStartLocation = aTimeStamp; }
    void incrementTimeStamp() { mDimensions[Time].mStartLocation += mDimensions[Time].mDelta * mDimensions[Time].mResolution; }
    void setFrequencyResolution(int aResolution, qreal aMaxFrequency);
    int getFrequencyResolution() const { return mDimensions[Frequency].mResolution; }
    };

} // namespace media

#endif // MEDIA_SPECTRUMFRAME_H
