#include "soundlinearpredictiontransform.h"
#include <QDebug>

namespace media {

SoundLinearPredictionTransform::SoundLinearPredictionTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mWeight(0.05)
    {
    mSoundFrame.setChannelsNo(2);
    mSoundFrame.setSampleBits(16);
    setProperty("tapsNo", 10);
    }

void SoundLinearPredictionTransform::process()
    {
    if (property("tapsNo").toInt() != mCoefficients.size())
        {
        mCoefficients.clear();
        mAudioSamplesTail.clear();
        for (int i = 0; i < property("tapsNo").toInt(); ++i)
            {
            double v = qrand();
            mCoefficients.push_back(0.5 - v/RAND_MAX);
            mAudioSamplesTail.push_back(0);
            }
        mFilterFrame.setResolution(property("tapsNo").toInt() + 1);
        }

    Q_ASSERT(mSourceElementsReadySet.size() == 1);
    const ElementBase* elem = *mSourceElementsReadySet.begin();
    Q_ASSERT(elem->getFramesNo() == 1);
    const FrameBase *frame = elem->getFrame(0);

    Q_ASSERT(frame->getMaxDimension() == SoundFrame::Dimensions);
    Q_ASSERT(mCoefficients.size() > 0);
    Q_ASSERT(frame->getDimensionT(SoundFrame::Time).mDelta);
    Q_ASSERT(frame->getDimensionT(SoundFrame::Time).mResolution > mCoefficients.size());

    mSoundFrame.setFrameSamples(frame->getDimensionT(SoundFrame::Time).mResolution);
    mSoundFrame.setSampleTime(frame->getDimensionT(SoundFrame::Time).mDelta);
    mSoundFrame.setTimeStamp(frame->getDimensionT(SoundFrame::Time).mStartLocation);
    mSoundFrame.setSourceName(frame->getSourceName());

    mFilterFrame.setSampleRate(frame->getDimensionT(SoundFrame::Time).mResolution/frame->getDimensionT(SoundFrame::Time).mDelta);
    mFilterFrame.setTimeStamp(frame->getDimensionT(SoundFrame::Time).mStartLocation);
    mFilterFrame.setSourceName(frame->getSourceName());
    mFilterFrame.setFrameSamples(1);

    int point[SoundFrame::Dimensions] = {0, 0};
    for (;point[SoundFrame::Time] < frame->getDimensionT(SoundFrame::Time).mResolution;
        ++point[SoundFrame::Time])
        {
        double res = 0;
        int shiftpoint[SoundFrame::Dimensions] = {0, 0};
        for (int shift = 1; shift <= mCoefficients.size(); ++shift)
            {
            shiftpoint[SoundFrame::Time] = point[SoundFrame::Time] - shift;
            res += mCoefficients.at(shift - 1) * (shiftpoint[SoundFrame::Time] < 0 ?
                mAudioSamplesTail.at(-shiftpoint[SoundFrame::Time] - 1) : frame->getSampleT(shiftpoint));
            }
        mSoundFrame.setSampleT(point, res);

        double diff = frame->getSampleT(point) - res;
        for (int shift = 1; shift <= mCoefficients.size(); ++shift)
            {
            shiftpoint[SoundFrame::Time] = point[SoundFrame::Time] - shift;
            mCoefficients[shift - 1] += 2*mWeight*diff*(shiftpoint[SoundFrame::Time] < 0 ?
                mAudioSamplesTail.at(-shiftpoint[SoundFrame::Time] - 1) : frame->getSampleT(shiftpoint));
            }
        shiftpoint[SoundFrame::Channels] = 1;
        shiftpoint[SoundFrame::Time] = point[SoundFrame::Time];
        mSoundFrame.setSampleT(shiftpoint, diff);
        }

    for (int shift = 1; shift <= mCoefficients.size(); ++shift)
        {
        point[SoundFrame::Time] = frame->getDimensionT(SoundFrame::Time).mResolution - shift;
        mAudioSamplesTail[shift - 1] =  frame->getSampleT(point);
        }

    int filterpoint[FilterFrame::Dimensions] = {0, 0, 0};

    filterpoint[FilterFrame::Polynomial] = FilterFrame::Numerator;
    mFilterFrame.setSampleT(filterpoint, 1);
    filterpoint[FilterFrame::Polynomial] = FilterFrame::Denominator;
    mFilterFrame.setSampleT(filterpoint, 1);

    for (filterpoint[FilterFrame::Coefficients] = 1;
         filterpoint[FilterFrame::Coefficients] <= mCoefficients.size();
       ++filterpoint[FilterFrame::Coefficients])
        {
        filterpoint[FilterFrame::Polynomial] = FilterFrame::Numerator;
        mFilterFrame.setSampleT(filterpoint, 0);
        filterpoint[FilterFrame::Polynomial] = FilterFrame::Denominator;
        mFilterFrame.setSampleT(filterpoint, -mCoefficients.at(filterpoint[FilterFrame::Coefficients] - 1));
        }

    emit framesReady();
    }

} // namespace media
