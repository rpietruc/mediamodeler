#include "alsaframe.h"

namespace media {

AlsaFrame::AlsaFrame(QObject *aParent) :
    QObject(aParent),
    FrameBase(Dimensions),
    mSoundBuffer(NULL),
    mBufSize(0)
    {
    mDimensions[Channels].mDelta = 1;
    }

AlsaFrame::~AlsaFrame()
    {
    delete [] mSoundBuffer;
    }

void AlsaFrame::resizeSoundBuffer()
    {
    if ((mDimensions[Channels].mResolution * mDimensions[Time].mResolution) != mBufSize)
        {
        delete [] mSoundBuffer;
        mBufSize = mDimensions[Channels].mResolution * mDimensions[Time].mResolution;
        mSoundBuffer = new qint16[mBufSize];
        memset(mSoundBuffer, 0, mBufSize);
        }
    }

void AlsaFrame::setChannelsNo(int aChannelsNo)
    {
    mDimensions[Channels].mResolution = aChannelsNo;
    resizeSoundBuffer();
    }

void AlsaFrame::setSampleTime(double aSampleTime)
    {
    mDimensions[Time].mDelta = aSampleTime;
    }

void AlsaFrame::setFrameTime(double aFrameTime)
    {
    if (mDimensions[Time].mDelta);
        {
        mDimensions[Time].mResolution = aFrameTime/mDimensions[Time].mDelta;
        resizeSoundBuffer();
        }
    }

void AlsaFrame::setTimeStamp(double aTimeStamp)
    {
    mDimensions[Time].mStartLocation = aTimeStamp;
    }

void AlsaFrame::incrementTimeStamp()
    {
    mDimensions[Time].mStartLocation += mDimensions[Time].mDelta * mDimensions[Time].mResolution;
    }

double AlsaFrame::getSample(const int *aPoint) const
    {
    Q_ASSERT(mSoundBuffer);
    Q_ASSERT(aPoint[Time] < mDimensions[Time].mResolution);
    Q_ASSERT(aPoint[Channels] < mDimensions[Channels].mResolution);

    double ret = mSoundBuffer[aPoint[Time]*mDimensions[Channels].mResolution + aPoint[Channels]];
    ret /= 32768.0;
    return ret;
    }

void AlsaFrame::setSample(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(mSoundBuffer);
    Q_ASSERT(aPoint[Time] < mDimensions[Time].mResolution);
    Q_ASSERT(aPoint[Channels] < mDimensions[Channels].mResolution);

    aValue *= 32768.0;
    mSoundBuffer[aPoint[Time]*mDimensions[Channels].mResolution + aPoint[Channels]] = aValue;
    }

void AlsaFrame::operator+=(const FrameBase &aFrame)
    {
    Q_ASSERT(aFrame.getMaxDimension() == Dimensions);

    //if buffer full count from the beginning
    if ((mDimensions[Time].mResolution * mDimensions[Channels].mResolution) == mBufSize)
        mDimensions[Time].mResolution = 0;

    int point[Dimensions];
    for (point[Time] = 0; point[Time] < aFrame.getDimension(Time).mResolution; ++point[Time])
        {
        int samplesNo = mDimensions[Time].mResolution * mDimensions[Channels].mResolution;
        for (point[Channels] = 0; point[Channels] < mDimensions[Channels].mResolution; ++point[Channels])
            {
            double sample = 0;
            if (point[Channels] < aFrame.getDimension(Channels).mResolution)
                sample = aFrame.getSample(point);
            Q_ASSERT(samplesNo < mBufSize);
            sample *= 32768.0;
            mSoundBuffer[samplesNo++] = sample;
            }

        ++mDimensions[Time].mResolution;
        if (samplesNo >= mBufSize)
            {
            emit bufferFilled();
            mDimensions[Time].mResolution = 0;
            }
        }
    }

} // namespace media
