#include "soundframe.h"

namespace media {

SoundFrame::SoundFrame(QObject *aParent) :
    QObject(aParent),
    FrameBase(Dimensions),
    mSampleBits(0),
    mSoundBuffer(MaxFrameSize, 0)
    {
    mDimensions[Channels].mDelta = 1.0;
    }

double SoundFrame::getSampleT(const int *aPoint) const
    {
    double value = 0.0;

    int sampleNo = aPoint[Time]*mDimensions[Channels].mResolution + aPoint[Channels];
    if ((sampleNo*mSampleBits/8) >= mSoundBuffer.size())
        return value;

    if (mSampleBits == 8)
        {
        value = mSoundBuffer.at(sampleNo);
        value /= 128.0;
        }
    else if (mSampleBits == 16)
        {
        value = reinterpret_cast<const qint16*>(mSoundBuffer.constData())[sampleNo];
        value /= 32768.0;
        }
    return value;
    }

void SoundFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    int sampleNo = aPoint[Time]*mDimensions[Channels].mResolution + aPoint[Channels];
    Q_ASSERT((sampleNo*mSampleBits/8) < mSoundBuffer.size());
    if (mSampleBits == 8)
        {
        aValue *= 128.0;
        mSoundBuffer[sampleNo] = aValue;
        }
    else if (mSampleBits == 16)
        {
        aValue *= 32768.0;
        reinterpret_cast<qint16*>(mSoundBuffer.data())[sampleNo] = aValue;
        }
    }

void SoundFrame::setSampleTime(double aSampleTime)
    {
    mDimensions[Time].mDelta = aSampleTime;
    }

void SoundFrame::setChannelsNo(int aChannelsNo)
    {
    Q_ASSERT((aChannelsNo * mDimensions[Time].mResolution * mSampleBits/8) <= mSoundBuffer.size());
    mDimensions[Channels].mResolution = aChannelsNo;
    }

void SoundFrame::setSampleBits(int aSampleBits)
    {
    Q_ASSERT((aSampleBits == 8) || (aSampleBits == 16));
    Q_ASSERT((mDimensions[Channels].mResolution * mDimensions[Time].mResolution * aSampleBits/8) <= mSoundBuffer.size());
    mSampleBits = aSampleBits;
    }

void SoundFrame::setFrameSize(int aBytesNo)
    {
    Q_ASSERT(aBytesNo <= mSoundBuffer.size());
    mDimensions[Time].mResolution = 8*aBytesNo/mDimensions[Channels].mResolution/mSampleBits;
    }

void SoundFrame::setFrameSamples(int aSampleNo)
    {
    Q_ASSERT(aSampleNo <= 8*mSoundBuffer.size()/mSampleBits);
    mDimensions[Time].mResolution = aSampleNo;
    }

int SoundFrame::getFrameSize() const
    {
    return mDimensions[Time].mResolution*mDimensions[Channels].mResolution*mSampleBits/8;
    }

bool SoundFrame::incrementFrameTime()
    {
    int newFrameSize = (mDimensions[Time].mResolution + 1)*mDimensions[Channels].mResolution*mSampleBits/8;
    if (newFrameSize > mSoundBuffer.size())
        return false;

    ++mDimensions[Time].mResolution;
    return true;
    }

void SoundFrame::eatBytes(int aSize)
    {
    Q_ASSERT(aSize < mSoundBuffer.size());
    Q_ASSERT((aSize % (mDimensions[Channels].mResolution*mSampleBits/8)) == 0);
    mSoundBuffer.resize(MaxFrameSize + aSize);
    mSoundBuffer.remove(0, aSize);
    mDimensions[Time].mResolution -= 8*aSize/mDimensions[Channels].mResolution/mSampleBits;
    Q_ASSERT(mSoundBuffer.size() == MaxFrameSize);
    }

void SoundFrame::operator+=(const FrameBase &aFrame)
    {
    Q_ASSERT(aFrame.getMaxDimension() == Dimensions);

    //if buffer full count from the beginning
    if (!getBytesFree())
        mDimensions[Time].mResolution = 0;

    int point[Dimensions] = {0, 0};
    int newpoint[Dimensions] = {mDimensions[Time].mResolution, 0};
    for (;point[Time] < aFrame.getDimensionT(Time).mResolution; ++point[Time], ++newpoint[Time])
        {
        point[Channels] = newpoint[Channels] = 0;
        if (!incrementFrameTime())
            {
            emit bufferFilled();
            mDimensions[Time].mResolution = 0;
            newpoint[Time] = 0;
            }

        for (; point[Channels] < mDimensions[Channels].mResolution; ++point[Channels], ++newpoint[Channels])
            {
            double sample = 0;
            if (point[Channels] < aFrame.getDimensionT(Channels).mResolution)
                sample = aFrame.getSampleT(point);
            setSampleT(newpoint, sample);
            }
        }
    }

const SoundFrame& SoundFrame::operator=(const FrameBase &aFrame)
    {
    Q_ASSERT(aFrame.getMaxDimension() == SoundFrame::Dimensions);

    mDimensions[Time].mDelta = aFrame.getDimensionT(Time).mDelta;
    mDimensions[Time].mResolution = aFrame.getDimensionT(Time).mResolution;
    mDimensions[Time].mStartLocation = aFrame.getDimensionT(Time).mStartLocation;
    setSourceName(aFrame.getSourceName());

    int point[Dimensions] = {0, 0};
    for (;point[Time] < aFrame.getDimensionT(Time).mResolution; ++point[Time])
        {
        for (point[Channels] = 0; point[Channels] < mDimensions[Channels].mResolution; ++point[Channels])
            {
            double sample = 0;
            if (point[Channels] < aFrame.getDimensionT(Channels).mResolution)
                sample = aFrame.getSampleT(point);
            setSampleT(point, sample);
            }
        }
    return *this;
    }
} // namespace media
