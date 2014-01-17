#ifndef MEDIA_SOUNDEXTRACTCHANNELTRANSFORM_H
#define MEDIA_SOUNDEXTRACTCHANNELTRANSFORM_H

#include "transformbase.h"
#include "soundframes.h"
#include <QtCore/qglobal.h>

#if defined(SOUNDEXTRACTCHANNELTRANSFORM_LIBRARY)
#  define SOUNDEXTRACTCHANNELTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SOUNDEXTRACTCHANNELTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace acoustics {
    class AdaptiveLpcRls;
}

namespace media {

class SOUNDEXTRACTCHANNELTRANSFORMSHARED_EXPORT SoundExtractChannelTransform : public TransformBase
{
    Q_OBJECT

    enum { LeftChannel = 0,
           RightChannel };

public:
    SoundExtractChannelTransform(TransformFactory *aFactory, const QString &aObjectName);

    virtual const FrameBase *getFrame() const { return &mChannelFrame; }

public slots:
    virtual void initSettings(const QString& aSettings);
    virtual void showSettingsDialog();

private slots:
    void setChannelIndex(int aChannelIndex);

private:
    virtual bool process();

    SoundFrame mChannelFrame;
    unsigned mChannelIndex;

    static const unsigned DefaultChannel = LeftChannel;

};

class SOUNDEXTRACTCHANNELTRANSFORMSHARED_EXPORT SoundExtractChannelTransformFactory : public TransformFactory
{
    Q_OBJECT
    Q_INTERFACES(media::TransformFactory)

protected:
    inline const char* getTransformName() const { return "SoundExtractChannelTransform"; }
    inline TransformBase* createTransform(const QString &aObjectName) { return new SoundExtractChannelTransform(this, aObjectName); }

};

} // namespace media

#endif // SOUNDEXTRACTCHANNELTRANSFORM_GLOBAL_H
