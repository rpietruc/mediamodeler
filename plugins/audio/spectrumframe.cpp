#include "spectrumframe.h"

namespace media {

void SpectrumFrame::setSampleRate(int aSampleRate)
    {
    Q_ASSERT(aSampleRate);
    mDimensions[Time].mDelta = 1.0/aSampleRate;
    }

void SpectrumFrame::setFrequencyResolution(int aResolution, qreal aMaxFrequency)
    {
    mDimensions[Frequency].mStartLocation = 0;
    mDimensions[Frequency].mDelta = aMaxFrequency / aResolution;
    }

void SpectrumFrame::operator+=(const FrameBase &aFrame)
    {
    Q_ASSERT(aFrame.getMaxDimension() == Dimensions);

    int point[Dimensions] = {0, 0};
    for (;point[Time] < aFrame.getDimensionT(Time).mResolution; ++point[Time])
        {
        Vector vect;
        point[Frequency] = 0;
        for (; point[Frequency] < mDimensions[Frequency].mResolution; ++point[Frequency])
            {
            double sample = 0;
            if (point[Frequency] < aFrame.getDimensionT(Frequency).mResolution)
                sample = aFrame.getSampleT(point);
            vect.append(sample);
            }

        ++mDimensions[Time].mResolution;
        bool res = setVector(vect, mDimensions[VectorNo].mResolution);
        if (!res) //buffer full
            {
            mDimensions[Time].mStartLocation += mDimensions[Time].mDelta*mDimensions[Time].mResolution;
            mDimensions[Time].mResolution = 1;
            setVectorT(vect, mDimensions[VectorNo].mResolution);
            }
        }
    }

} // namespace media
