#ifndef MEDIA_FORMANTFRAME_H
#define MEDIA_FORMANTFRAME_H

#include "matrixframe.h"

namespace media {

class FormantFrame : public MatrixFrame
    {
public:
    enum
        {
        Time = VectorNo, // 0
        Formants = ParamNo, // 1
        Dimensions
        };

    explicit FormantFrame();

    inline void setFrameSamples(int aVectorNo) { setMaxVectors(aVectorNo); }
    void setSampleRate(int aSampleRate);
    inline void setTimeStamp(double aTimeStamp) { mDimensions[Time].mStartLocation = aTimeStamp; }
    inline void incrementTimeStamp() { mDimensions[Time].mStartLocation += mDimensions[Time].mDelta * mDimensions[Time].mResolution; }
    inline void setFormantsNoT(int aMaxFormants) { setVectorSizeT(aMaxFormants); }

private:
    QString mSourceName;
    };

} // namespace media

#endif // MEDIA_FORMANTFRAME_H
