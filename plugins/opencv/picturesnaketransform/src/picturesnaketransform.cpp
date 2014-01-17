#include "picturesnaketransform.h"
#include "pictureframes.h"
#include <QtPlugin>
#include <opencv2/legacy/legacy.hpp>

#define pi 3.14159265358979

namespace media {

PictureSnakeTransform::PictureSnakeTransform(TransformFactory *aFactory, const QString &aObjectName) :
    TransformBase(aFactory, aObjectName),
    mPointsNo(0),
    mNeighborhoodSize(11),
    mContinuityEnergyWeight(0.35),
    mCurvatureEnergyWeight(0.21),
    mImageEnergyWeight(0.62)
{
}

void PictureSnakeTransform::showSettingsDialog()
{
    setPointsNo(20);
}

void PictureSnakeTransform::setPointsNo(int aPointsNo)
{
    mPointsNo = aPointsNo;
    emit settingsChanged(QString("%1").arg(aPointsNo));
}

void PictureSnakeTransform::initSettings(const QString& aSettings)
{
    setPointsNo(aSettings.toInt());
}

bool PictureSnakeTransform::process()
{
    const PictureFrame *pictureFrame = NULL;
    const RegionFrame *regionFrame = NULL;
    foreach (const FrameBase *frame, mCurrentFramesReadySet)
    {
        if (!pictureFrame)
            pictureFrame = qobject_cast<const PictureFrame*>(frame);
        if (!regionFrame)
            regionFrame = qobject_cast<const RegionFrame*>(frame);
    }
    Q_ASSERT(pictureFrame);
    cv::Point center;
    cv::Size radius;
    if (regionFrame)
    {
        Q_ASSERT(regionFrame->mRectVector.size() > 0);

        center.x = regionFrame->mRectVector[0].x + regionFrame->mRectVector[0].width/2;
        center.y = regionFrame->mRectVector[0].y + regionFrame->mRectVector[0].height/2;

        radius.width = regionFrame->mRectVector[0].width/2;
        radius.height = regionFrame->mRectVector[0].height/2;
    }
    else
    {
        center.x = pictureFrame->mIplImage->width/2;
        center.y = pictureFrame->mIplImage->height/2;

        radius.width = pictureFrame->mIplImage->width/4;
        radius.height = pictureFrame->mIplImage->height/4;
    }
    for (int i = 0; i < mPointsNo; ++i)
    {
        mPointsFrame.mPointVector[i].x = center.x + static_cast<int>(round(radius.width*cos(i*((2*pi)/mPointsNo))));
        mPointsFrame.mPointVector[i].y = center.y - radius.height + static_cast<int>(round(radius.height*sin(i*((2*pi)/mPointsNo))));
    }
    if (pictureFrame)
    {
        Q_ASSERT(pictureFrame->isValid());
        CvSize searchWindow = {mNeighborhoodSize, mNeighborhoodSize};
        CvTermCriteria termCriteria = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1.e-15f);
        cvSnakeImage(pictureFrame->mIplImage, mPointsFrame.mPointVector, mPointsNo, &mContinuityEnergyWeight, &mCurvatureEnergyWeight, &mImageEnergyWeight, CV_VALUE, searchWindow, termCriteria, 0);
        mPointsFrame.mPointsNo = mPointsNo;
    }
    return true;
}

Q_EXPORT_PLUGIN2(PLUGINTARGETNAME, PictureSnakeTransformFactory);

} // namespace media

