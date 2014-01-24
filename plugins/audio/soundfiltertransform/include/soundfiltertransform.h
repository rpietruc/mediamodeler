#ifndef MEDIA_SOUNDFILTERTRANSFORM_H
#define MEDIA_SOUNDFILTERTRANSFORM_H

#include "elementbase.h"
#include "soundframe.h"
#include <QList>
#include <QtCore/qglobal.h>

#if defined(SOUNDFILTERTRANSFORM_LIBRARY)
#  define SOUNDFILTERTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SOUNDFILTERTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace acoustics {
    class AdaptiveLpcBase;
}

namespace media {

class SOUNDFILTERTRANSFORMSHARED_EXPORT SoundFilterTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit SoundFilterTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mSoundFrame; }

    ParamList getParams() const;

public slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private:
    void setFilter(const QStringList &aParamList);
    void process();

    QList<double> mTail;
    QList<double> mFilter;
    SoundFrame mSoundFrame;
    };

class SOUNDFILTERTRANSFORMSHARED_EXPORT SoundFilterTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "SoundFilterTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new SoundFilterTransform(this, aObjectName); }
    };

} // namespace media

#endif // SOUNDFILTERTRANSFORM_GLOBAL_H
