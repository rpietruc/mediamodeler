#include "pictureplottransform.h"
#include "pictureframes.h"
#include <QtPlugin>

namespace media {

PicturePlotTransform::PicturePlotTransform(TransformFactory *aFactory, const QString &aObjectName) :
    TransformBase(aFactory, aObjectName),
    mMaxValue(0.0)
{
}

void PicturePlotTransform::showSettingsDialog()
{
    setMaxValue(0.0);
}

void PicturePlotTransform::setMaxValue(int aMaxValue)
{
    mMaxValue = aMaxValue;
    emit settingsChanged(QString("%1").arg(aMaxValue));
}

void PicturePlotTransform::initSettings(const QString& aSettings)
{
    setMaxValue(aSettings.toInt());
}

bool PicturePlotTransform::process()
{
    const PlotFrame *plotFrame = NULL;
    foreach (const FrameBase *frame, mCurrentFramesReadySet)
        if ((plotFrame = qobject_cast<const PlotFrame*>(frame)) != NULL)
            break;

    Q_ASSERT(plotFrame);
    if (plotFrame)
    {
        Q_ASSERT(plotFrame->isValid());
        if (!mPictureFrame.isValid() || (mPictureFrame.mIplImage->width != plotFrame->mValues.size()))
        {
            if (mPictureFrame.isValid())
                mPictureFrame.release();
            mPictureFrame.mIplImage = cvCreateImage(cvSize(plotFrame->mValues.size(), plotFrame->mValues.size()), IPL_DEPTH_8U, 1);
        }
        cvSet(mPictureFrame.mIplImage, cvScalar(255));

        for (unsigned x = 0; x < plotFrame->mValues.size(); ++x)
            if (plotFrame->mValues[x] > mMaxValue)
                mMaxValue = plotFrame->mValues[x];

        if (mMaxValue > 0.0)
        {
            // plot points
            unsigned x0 = 0;
            unsigned y1 = plotFrame->mValues.size() - plotFrame->mValues.size()*(plotFrame->mValues[x0]/mMaxValue);
            for (unsigned x1 = 1; x1 < plotFrame->mValues.size(); ++x1, ++x0)
            {
                unsigned y0 = y1;
                y1 = plotFrame->mValues.size() - plotFrame->mValues.size()*(plotFrame->mValues[x1]/mMaxValue);
                cvLine(mPictureFrame.mIplImage, cvPoint(x0, y0), cvPoint(x1, y1), cvScalar(0));
            }
        }
    }
    else
        return false;

    return true;
}

Q_EXPORT_PLUGIN2(PLUGINTARGETNAME, PicturePlotTransformFactory);

} // namespace media
