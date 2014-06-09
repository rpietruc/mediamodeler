#ifndef MEDIA_FORMANTFRAME_H
#define MEDIA_FORMANTFRAME_H

#include "matrixframe.h"

namespace media {

class FormantFrame : public MatrixFrame
    {
public:
    enum
        {
        Time = SampleNo, // 0
        Formants = VectorNo, // 1
        Dimensions
        };

    explicit FormantFrame();

    inline void setFrameSamples(int aSampleNo) { setMaxSamplesT(aSampleNo); }
    void setSampleRate(int aSampleRate);
    inline void setTimeStamp(double aTimeStamp) { mDimensions[Time].mStartLocation = aTimeStamp; }
    inline void incrementTimeStamp() { mDimensions[Time].mStartLocation += mDimensions[Time].mDelta * mDimensions[Time].mResolution; }

    inline void setFormantsNo(int aMaxFormants) { setMaxVectorsT(aMaxFormants); }

private:
    QString mSourceName;
    };

} // namespace media

#endif // MEDIA_FORMANTFRAME_H
