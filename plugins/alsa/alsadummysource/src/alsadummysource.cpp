#include "alsadummysource.h"
#include <QDateTime>
#include <qmath.h>

namespace media {

#define Pi 3.14159265358979

struct SingleFrequencySignal
    {
    double mFrequency;
    double mAmplitude;
    double mPhase;

    SingleFrequencySignal(double aFrequency, double aAmplitude, double aPhase) :
        mFrequency(aFrequency),
        mAmplitude(aAmplitude),
        mPhase(aPhase)
        {}

    qreal getSample(double aTime)
        {
        return mAmplitude*sin(2*Pi*(aTime*mFrequency + mPhase));
        }
    };

AlsaDummySource::AlsaDummySource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mSignal(new SingleFrequencySignal(1.0, 0.5, 0.0))
    {
    mAlsaFrame.setSourceName("dummy");
    mAlsaFrame.setChannelsNo(1);
    mAlsaFrame.setSampleRate(8000);
    mAlsaFrame.setFrameTime(AlsaFrame::DefaultFrameTime);
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
    mAlsaFrame.setTimeStamp(0);
#else
    mAlsaFrame.setTimeStamp(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);
#endif
    }

//ElementBase::ParamList AlsaDummySource::getParams() const
//    {
//    ParamList ret;
//    ret["Frequency"] = mSignal->mFrequency;
//    ret["Amplitude"] = mSignal->mAmplitude;
//    ret["Phase"] = mSignal->mPhase;
//    return ret;
//    }

//void AlsaDummySource::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    if (aName == "Frequency")
//        mSignal->mFrequency = aValue.toDouble();
//    else if (aName == "Amplitude")
//        mSignal->mAmplitude = aValue.toDouble();
//    else if (aName == "Phase")
//        mSignal->mPhase = aValue.toDouble();
//    }

void AlsaDummySource::process()
    {
    int point[AlsaFrame::Dimensions] = {0, 0};
    double timeStamp = mAlsaFrame.getDimension(AlsaFrame::Time).mStartLocation;
    for (point[AlsaFrame::Time] = 0;
         point[AlsaFrame::Time] < mAlsaFrame.getDimension(AlsaFrame::Time).mResolution;
       ++point[AlsaFrame::Time], timeStamp += mAlsaFrame.getDimension(AlsaFrame::Time).mDelta)
        mAlsaFrame.setSample(point, mSignal->getSample(timeStamp));
    mAlsaFrame.incrementTimeStamp();
    //mAlsaFrame.setTimeStamp(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);

    emit framesReady();
    }

} // namespace media
