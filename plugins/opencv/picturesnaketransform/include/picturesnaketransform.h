#ifndef PICTURESNAKETRANSFORM_H
#define PICTURESNAKETRANSFORM_H

#include "transformbase.h"
#include "pictureframes.h"
#include <QtCore/qglobal.h>

#if defined(PICTURESNAKETRANSFORM_LIBRARY)
#  define PICTURESNAKETRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTURESNAKETRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTURESNAKETRANSFORMSHARED_EXPORT PictureSnakeTransform : public TransformBase
{
    Q_OBJECT

public:
    PictureSnakeTransform(TransformFactory *aFactory, const QString &aObjectName);
    inline const FrameBase *getFrame() const { return &mPointsFrame; }

public slots:
    virtual void initSettings(const QString& aSettings);
    virtual void showSettingsDialog();

private slots:
    void setPointsNo(int aPointsNo);

private:
    virtual bool process();

    PointsFrame mPointsFrame;

    // number of snake points
    int mPointsNo;

    // neighborhood size, must be odd
    const int mNeighborhoodSize;

    // weight of continuity energy
    // continuity energy refers to the distance between a pixel
    // and its neighboring snake point with respect to all the snake points;
    // it is calculated by subtracting the average distance
    // (between all snake points)
    // from the distance between the pixel and the previous snake point
    float mContinuityEnergyWeight;

    // weight of curvature energy
    // curvature energy refers to the amount curvature between a pixel
    // and the neighboring snake points
    // this can be calculated using the second derivative, which corresponds to:
    // (next snake point - pixel) - (pixel - previous snake point)
    float mCurvatureEnergyWeight;

    // weight of image energy
    // gradient energy merely refers to negative
    // the size of the gradient of the pixel
    float mImageEnergyWeight;
};

class PICTURESNAKETRANSFORMSHARED_EXPORT PictureSnakeTransformFactory : public TransformFactory
{
    Q_OBJECT
    Q_INTERFACES(media::TransformFactory)

protected:
    inline const char* getTransformName() const { return "PictureSnakeTransform"; }
    inline TransformBase* createTransform(const QString &aObjectName) { return new PictureSnakeTransform(this, aObjectName); }

};

} // namespace media

#endif // PICTURESNAKETRANSFORM_H
