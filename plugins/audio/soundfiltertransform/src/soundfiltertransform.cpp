#include "soundfiltertransform.h"
#include <QDebug>

using namespace acoustics;

namespace media {

SoundFilterTransform::SoundFilterTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    Q_ASSERT((sizeof(mFilter)/sizeof(mFilter[0])) == (sizeof(mTail)/sizeof(mTail[0])));
    // preemphasis: 1, -0.9735
    mSoundFrame.setChannelsNo(1);
    for (int i = 0; i < (int)(sizeof(mFilter)/sizeof(mFilter[0])); ++i)
        setProperty(qPrintable(QString("a%1").arg(i)), 0);
    memset(mTail, 0, sizeof(mTail));
    }

void SoundFilterTransform::process()
    {
    for (int i = 0; i < (int)(sizeof(mFilter)/sizeof(mFilter[0])); ++i)
        mFilter[i] = property(qPrintable(QString("a%1").arg(i))).toDouble();

    Q_ASSERT(mSourceElementsReadySet.size() == 1);
    const ElementBase* elem = *mSourceElementsReadySet.begin();
    Q_ASSERT(elem->getFramesNo() == 1);
    const FrameBase *frame = elem->getFrame(0);

    Q_ASSERT(frame->getMaxDimension() == SoundFrame::Dimensions);
    Q_ASSERT(frame->getDimension(SoundFrame::Time).mDelta);
    Q_ASSERT(frame->getDimension(SoundFrame::Time).mResolution > (int)(sizeof(mFilter)/sizeof(mFilter[0])));

    mSoundFrame.setSampleBits(16);
    mSoundFrame.setFrameSamples(frame->getDimension(SoundFrame::Time).mResolution);
    mSoundFrame.setSampleTime(frame->getDimension(SoundFrame::Time).mDelta);
    mSoundFrame.setSourceName(frame->getSourceName());
    mSoundFrame.setTimeStamp(frame->getDimension(SoundFrame::Time).mStartLocation);

    int point[SoundFrame::Dimensions] = {0, 0};
    for (;point[SoundFrame::Time] < frame->getDimension(SoundFrame::Time).mResolution; ++point[SoundFrame::Time])
        {
        double res = 0;
        for (int i = 0; i < (int)(sizeof(mFilter)/sizeof(mFilter[0])); ++i)
            {
            if ((point[SoundFrame::Time] - i) < 0)
                res += mFilter[i] * mTail[i - point[SoundFrame::Time] - 1];
            else
                res += frame->getSample(point) * mFilter[i];
            }
        mSoundFrame.setSample(point, res);
        }
    for (int i = 0; i < (int)(sizeof(mFilter)/sizeof(mFilter[0]) - 1); ++i)
        {
        --point[SoundFrame::Time];
        mTail[i] = frame->getSample(point);
        }
    emit framesReady();
    }

} // namespace media
