#include "spectrumframe.h"

namespace media {

SpectrumFrame::SpectrumFrame()
    {
    }

void SpectrumFrame::setSampleRate(int aSampleRate)
    {
    Q_ASSERT(aSampleRate);
    mDimensions[Time].mDelta = 1.0/aSampleRate;
    }

void SpectrumFrame::setResolution(int aResolution, qreal aMaxFrequency)
    {
    mDimensions[Frequency].mStartLocation = 0;
    mDimensions[Frequency].mDelta = aMaxFrequency / aResolution;
    setMaxVectorsT(aResolution);
    }

void SpectrumFrame::operator+=(const FrameBase &aFrame)
    {
    Q_ASSERT(aFrame.getMaxDimension() == Dimensions);

    int point[Dimensions] = {0, 0};
    int newpoint[Dimensions] = {mDimensions[Time].mResolution, 0};
    for (;point[Time] < aFrame.getDimensionT(Time).mResolution; ++point[Time], ++newpoint[Time])
        {
        //if buffer full count from the beginning
        Q_ASSERT(mMatrix.size() > 0);
        if (mDimensions[SampleNo].mResolution >= mMatrix[0].size())
            {
            mDimensions[Time].mStartLocation += mDimensions[Time].mDelta*mDimensions[Time].mResolution;
            mDimensions[Time].mResolution = 1;
            newpoint[Time] = 0;
            }
        else
            ++mDimensions[Time].mResolution;

        point[Frequency] = newpoint[Frequency] = 0;
        for (; point[Frequency] < mDimensions[Frequency].mResolution; ++point[Frequency], ++newpoint[Frequency])
            {
            double sample = 0;
            if (point[Frequency] < aFrame.getDimensionT(Frequency).mResolution)
                sample = aFrame.getSampleT(point);
            setSampleT(newpoint, sample);
            }
        }
    }

} // namespace media
