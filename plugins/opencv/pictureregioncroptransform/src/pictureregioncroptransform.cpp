#include "pictureregioncroptransform.h"
#include <QtPlugin>

namespace media {

PictureRegionCropTransform::PictureRegionCropTransform(TransformFactory *aFactory, const QString &aObjectName) :
    TransformBase(aFactory, aObjectName)
{
}

bool PictureRegionCropTransform::process()
{
    const RegionFrame *regionFrame = NULL;
    const PictureFrame *pictureFrame = NULL;
    foreach (const FrameBase *frame, mCurrentFramesReadySet)
    {
        if (!pictureFrame)
            pictureFrame = qobject_cast<const PictureFrame*>(frame);
        if (!regionFrame)
            regionFrame = qobject_cast<const RegionFrame*>(frame);
    }
    Q_ASSERT(regionFrame && pictureFrame);
    if (regionFrame->mRectVector.size() > 0)
    {
        if (!mPictureFrame.isValid() || (regionFrame->mRectVector[0].size().width != mPictureFrame.mIplImage->width) || (regionFrame->mRectVector[0].size().height != mPictureFrame.mIplImage->height))
        {
            if (mPictureFrame.isValid())
                mPictureFrame.release();

            mPictureFrame.mIplImage = cvCreateImage(cvSize(regionFrame->mRectVector[0].size().width, regionFrame->mRectVector[0].size().height), pictureFrame->mIplImage->depth, pictureFrame->mIplImage->nChannels);
        }
        cvSetImageROI(pictureFrame->mIplImage, regionFrame->mRectVector[0]);
        cvCopy(pictureFrame->mIplImage, mPictureFrame.mIplImage);
        cvResetImageROI(pictureFrame->mIplImage);
    }
    return true;
}

Q_EXPORT_PLUGIN2(PLUGINTARGETNAME, PictureRegionCropTransformFactory);

} // namespace media

