#ifndef MEDIA_PICTUREREGIONCROPTRANSFORM_H
#define MEDIA_PICTUREREGIONCROPTRANSFORM_H

#include "pictureframes.h"
#include "transformbase.h"
#include <opencv/cv.h>
#include <QtCore/qglobal.h>

#if defined(PICTUREREGIONCROPTRANSFORM_LIBRARY)
#  define PICTUREREGIONCROPTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREREGIONCROPTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PictureRegionCropTransform : public TransformBase
{
    Q_OBJECT

public:
    PictureRegionCropTransform(TransformFactory *aFactory, const QString &aObjectName);
    inline virtual const FrameBase *getFrame() const { return &mPictureFrame; }

private:
    virtual bool process();

    PictureFrame mPictureFrame;

};

class PictureRegionCropTransformFactory : public TransformFactory
{
    Q_OBJECT
    Q_INTERFACES(media::TransformFactory)

protected:
    inline const char* getTransformName() const { return "PictureRegionCropTransform"; }
    inline TransformBase* createTransform(const QString &aObjectName) { return new PictureRegionCropTransform(this, aObjectName); }

};

} // namespace media

#endif // MEDIA_PICTUREREGIONCROPTRANSFORM_H
