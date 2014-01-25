#ifndef MEDIA_SOUNDLINEARPREDICTIONTRANSFORM_H
#define MEDIA_SOUNDLINEARPREDICTIONTRANSFORM_H

#include "elementbase.h"
#include "soundframe.h"
#include "filterframe.h"
#include <QVector>
#include <QtCore/qglobal.h>

#if defined(SOUNDLINEARPREDICTIONTRANSFORM_LIBRARY)
#  define SOUNDLINEARPREDICTIONTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SOUNDLINEARPREDICTIONTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace acoustics {
    class AdaptiveLpcBase;
}

namespace media {

class SOUNDLINEARPREDICTIONTRANSFORMSHARED_EXPORT SoundLinearPredictionTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit SoundLinearPredictionTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 2; }
    const FrameBase *getFrame(int aIndex) const
        {
        if (aIndex)
            return &mSoundFrame;
        //else
            return &mFilterFrame;
        }

//    ParamList getParams() const;

public slots:
//    void setParamValue(const QString& aName, const QVariant& aValue);

private:
    void setTaps(int aTaps);
    void process();

    QVector<double> mCoefficients;
    QVector<double> mAudioSamplesTail;
    SoundFrame mSoundFrame;
    FilterFrame mFilterFrame;
    double mWeight;
    };

class SOUNDLINEARPREDICTIONTRANSFORMSHARED_EXPORT SoundLinearPredictionTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "SoundLinearPredictionTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new SoundLinearPredictionTransform(this, aObjectName); }
    };

} // namespace media

#endif // SOUNDLINEARPREDICTIONTRANSFORM_GLOBAL_H
