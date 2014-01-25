#include "soundlinearpredictiontransform.h"
#include <QDebug>

namespace media {

SoundLinearPredictionTransform::SoundLinearPredictionTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mWeight(0.05)
    {
    mSoundFrame.setChannelsNo(2);
    mSoundFrame.setSampleBits(16);
    setTaps(10);
    }

//ElementBase::ParamList SoundLinearPredictionTransform::getParams() const
//    {
//    ParamList ret;
//    ret["Taps No"] = mCoefficients.size();
//    return ret;
//    }

//void SoundLinearPredictionTransform::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    Q_UNUSED(aName);
//    if (mCoefficients.size() != aValue.toInt())
//        setTaps(aValue.toInt());
//    }

void SoundLinearPredictionTransform::setTaps(int aTaps)
    {
    mCoefficients.clear();
    mAudioSamplesTail.clear();
    for (int i = 0; i < aTaps; ++i)
        {
        double v = qrand();
        mCoefficients.push_back(0.5 - v/RAND_MAX);
        mAudioSamplesTail.push_back(0);
        }
    mFilterFrame.setResolution(aTaps + 1);
    }

void SoundLinearPredictionTransform::process()
    {
    Q_ASSERT(mSourceElementsReadySet.size() == 1);
    const ElementBase* elem = *mSourceElementsReadySet.begin();
    Q_ASSERT(elem->getFramesNo() == 1);
    const FrameBase *frame = elem->getFrame(0);

    Q_ASSERT(frame->getMaxDimension() == SoundFrame::Dimensions);
    Q_ASSERT(mCoefficients.size() > 0);
    Q_ASSERT(frame->getDimension(SoundFrame::Time).mDelta);
    Q_ASSERT(frame->getDimension(SoundFrame::Time).mResolution > mCoefficients.size());

    mSoundFrame.setFrameSamples(frame->getDimension(SoundFrame::Time).mResolution);
    mSoundFrame.setSampleRate(1/frame->getDimension(SoundFrame::Time).mDelta);
    mSoundFrame.setTimeStamp(frame->getDimension(SoundFrame::Time).mStartLocation);
    mSoundFrame.setSourceName(frame->getSourceName());

    mFilterFrame.setSampleRate(frame->getDimension(SoundFrame::Time).mResolution/frame->getDimension(SoundFrame::Time).mDelta);
    mFilterFrame.setTimeStamp(frame->getDimension(SoundFrame::Time).mStartLocation);
    mFilterFrame.setSourceName(frame->getSourceName());
    mFilterFrame.setFrameSamples(1);

    int point[SoundFrame::Dimensions] = {0, 0};
    for (;point[SoundFrame::Time] < frame->getDimension(SoundFrame::Time).mResolution;
        ++point[SoundFrame::Time])
        {
        double res = 0;
        int shiftpoint[SoundFrame::Dimensions] = {0, 0};
        for (int shift = 1; shift <= mCoefficients.size(); ++shift)
            {
            shiftpoint[SoundFrame::Time] = point[SoundFrame::Time] - shift;
            res += mCoefficients.at(shift - 1) * (shiftpoint[SoundFrame::Time] < 0 ?
                mAudioSamplesTail.at(-shiftpoint[SoundFrame::Time] - 1) : frame->getSample(shiftpoint));
            }
        mSoundFrame.setSample(point, res);

        double diff = frame->getSample(point) - res;
        for (int shift = 1; shift <= mCoefficients.size(); ++shift)
            {
            shiftpoint[SoundFrame::Time] = point[SoundFrame::Time] - shift;
            mCoefficients[shift - 1] += 2*mWeight*diff*(shiftpoint[SoundFrame::Time] < 0 ?
                mAudioSamplesTail.at(-shiftpoint[SoundFrame::Time] - 1) : frame->getSample(shiftpoint));
            }
        shiftpoint[SoundFrame::Channels] = 1;
        shiftpoint[SoundFrame::Time] = point[SoundFrame::Time];
        mSoundFrame.setSample(shiftpoint, diff);
        }

    for (int shift = 1; shift <= mCoefficients.size(); ++shift)
        {
        point[SoundFrame::Time] = frame->getDimension(SoundFrame::Time).mResolution - shift;
        mAudioSamplesTail[shift - 1] =  frame->getSample(point);
        }

    int filterpoint[FilterFrame::Dimensions] = {0, 0, 0};

    filterpoint[FilterFrame::Polynomial] = FilterFrame::Numerator;
    mFilterFrame.setSample(filterpoint, 1);
    filterpoint[FilterFrame::Polynomial] = FilterFrame::Denominator;
    mFilterFrame.setSample(filterpoint, 1);

    for (filterpoint[FilterFrame::Coefficients] = 1;
         filterpoint[FilterFrame::Coefficients] <= mCoefficients.size();
       ++filterpoint[FilterFrame::Coefficients])
        {
        filterpoint[FilterFrame::Polynomial] = FilterFrame::Numerator;
        mFilterFrame.setSample(filterpoint, 0);
        filterpoint[FilterFrame::Polynomial] = FilterFrame::Denominator;
        mFilterFrame.setSample(filterpoint, -mCoefficients.at(filterpoint[FilterFrame::Coefficients] - 1));
        }

    emit framesReady();
    }

} // namespace media
