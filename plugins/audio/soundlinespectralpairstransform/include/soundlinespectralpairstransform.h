#ifndef SOUNDLINESPECTRALPAIRSTRANSFORM_H
#define SOUNDLINESPECTRALPAIRSTRANSFORM_H

#include "transformbase.h"
#include "soundframes.h"
#include <QtCore/qglobal.h>

#if defined(SOUNDLINESPECTRALPAIRSTRANSFORM_LIBRARY)
#  define SOUNDLINESPECTRALPAIRSTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SOUNDLINESPECTRALPAIRSTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class SOUNDLINESPECTRALPAIRSTRANSFORMSHARED_EXPORT SoundLineSpectralPairsTransform : public TransformBase
{
    Q_OBJECT

public:
    SoundLineSpectralPairsTransform(TransformFactory *aFactory, const QString &aObjectName);

    virtual const FrameBase *getFrame() const { return &mLPCFrame; }

private:
    virtual bool process();

    LinearPredictionCoefficientsFrame mLPCFrame;

};

class SOUNDLINESPECTRALPAIRSTRANSFORMSHARED_EXPORT SoundLineSpectralPairsTransformFactory : public TransformFactory
{
    Q_OBJECT
    Q_INTERFACES(media::TransformFactory)

protected:
    inline const char* getTransformName() const { return "SoundLineSpectralPairsTransform"; }
    inline TransformBase* createTransform(const QString &aObjectName) { return new SoundLineSpectralPairsTransform(this, aObjectName); }

};

} // namespace media

#endif // SOUNDLINESPECTRALPAIRSTRANSFORM_H
