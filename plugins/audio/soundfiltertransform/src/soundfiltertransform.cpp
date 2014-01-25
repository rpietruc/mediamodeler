#include "soundfiltertransform.h"
#include <QDebug>

using namespace acoustics;

namespace media {

SoundFilterTransform::SoundFilterTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    mSoundFrame.setChannelsNo(1);

    // preemphasis
    mFilter << 1 << -0.9735;
    mTail << 0 << 0;
    }

//ElementBase::ParamList SoundFilterTransform::getParams() const
//    {
//    ParamList ret;
//    ret["a0"] = mFilter.at(0);
//    ret["a1"] = mFilter.at(1);
//    return ret;
//    }

//void SoundFilterTransform::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    if (aName == "a0")
//        mFilter[0] = aValue.toDouble();
//    else if (aName == "a1")
//        mFilter[1] = aValue.toDouble();
//    }

void SoundFilterTransform::process()
    {
    Q_ASSERT(mSourceElementsReadySet.size() == 1);
    const ElementBase* elem = *mSourceElementsReadySet.begin();
    Q_ASSERT(elem->getFramesNo() == 1);
    const FrameBase *frame = elem->getFrame(0);

    Q_ASSERT(frame->getMaxDimension() == SoundFrame::Dimensions);
    Q_ASSERT(mFilter.size() > 0);
    Q_ASSERT(frame->getDimension(SoundFrame::Time).mDelta);
    Q_ASSERT(frame->getDimension(SoundFrame::Time).mResolution > mFilter.size());

    mSoundFrame.setSampleBits(16);
    mSoundFrame.setFrameSamples(frame->getDimension(SoundFrame::Time).mResolution);
    mSoundFrame.setSampleRate(1/frame->getDimension(SoundFrame::Time).mDelta);
    mSoundFrame.setSourceName(frame->getSourceName());
    mSoundFrame.setTimeStamp(frame->getDimension(SoundFrame::Time).mStartLocation);

    int point[SoundFrame::Dimensions] = {0, 0};
    for (;point[SoundFrame::Time] < frame->getDimension(SoundFrame::Time).mResolution; ++point[SoundFrame::Time])
        {
        double res = 0;
        for (int i = 0; i < mFilter.size(); ++i)
            {
            if ((point[SoundFrame::Time] - i) < 0)
                res += mFilter[i] * mTail[i - point[SoundFrame::Time] - 1];
            else
                res += frame->getSample(point) * mFilter[i];
            }
        mSoundFrame.setSample(point, res);
        }
    for (int i = 0; i < (mFilter.size() - 1); ++i)
        {
        --point[SoundFrame::Time];
        mTail[i] = frame->getSample(point);
        }
    emit framesReady();
    }

} // namespace media
