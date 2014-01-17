#ifndef MEDIA_PICTUREPLOTTRANSFORM_H
#define MEDIA_PICTUREPLOTTRANSFORM_H

#include "transformbase.h"
#include "pictureframes.h"
#include <QtCore/qglobal.h>

#if defined(PICTUREPLOTTRANSFORM_LIBRARY)
#  define PICTUREPLOTTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREPLOTTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREPLOTTRANSFORMSHARED_EXPORT PicturePlotTransform : public TransformBase
{
    Q_OBJECT

public:
    PicturePlotTransform(TransformFactory *aFactory, const QString &aObjectName);
    inline virtual const FrameBase *getFrame() const { return &mPictureFrame; }

public slots:
    void showSettingsDialog();
    void initSettings(const QString& aSettings);

private slots:
    void setMaxValue(int aMaxValue);

private:
    virtual bool process();

    PictureFrame mPictureFrame;
    float mMaxValue;

};

class PICTUREPLOTTRANSFORMSHARED_EXPORT PicturePlotTransformFactory : public TransformFactory
{
    Q_OBJECT
    Q_INTERFACES(media::TransformFactory)

protected:
    inline const char* getTransformName() const { return "PicturePlotTransform"; }
    inline TransformBase* createTransform(const QString &aObjectName) { return new PicturePlotTransform(this, aObjectName); }

};

} // namespace media

#endif // MEDIA_PICTUREPLOTTRANSFORM_H
