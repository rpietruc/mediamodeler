#include "alsadummysource.h"
#include <QDateTime>
#include <qmath.h>

namespace media {

#define Pi 3.14159265358979

qreal getSample(double aTime, double aFrequency, double aAmplitude, double aPhase)
    {
    return aAmplitude*sin(2*Pi*(aTime*aFrequency + aPhase));
    }

AlsaDummySource::AlsaDummySource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("frequency", 1000);
    setProperty("amplitude", 1.0);
    setProperty("phase", 0.0);

    mAlsaFrame.setSourceName("dummy");
    mAlsaFrame.setChannelsNo(1);
    mAlsaFrame.setSampleTime(1.0/8000);
    mAlsaFrame.setFrameTime(AlsaFrame::DefaultFrameTime);
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
    mAlsaFrame.setTimeStamp(0);
#else
    mAlsaFrame.setTimeStamp(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);
#endif
    }

void AlsaDummySource::process()
    {
    int point[AlsaFrame::Dimensions] = {0, 0};
    double timeStamp = mAlsaFrame.getDimension(AlsaFrame::Time).mStartLocation;
    for (point[AlsaFrame::Time] = 0;
         point[AlsaFrame::Time] < mAlsaFrame.getDimension(AlsaFrame::Time).mResolution;
       ++point[AlsaFrame::Time], timeStamp += mAlsaFrame.getDimension(AlsaFrame::Time).mDelta)
        mAlsaFrame.setSample(point, getSample(timeStamp, property("frequency").toDouble(), property("amplitude").toDouble(), property("phase").toDouble()));
    mAlsaFrame.incrementTimeStamp();
    //mAlsaFrame.setTimeStamp(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);

    emit framesReady();
    }

} // namespace media
