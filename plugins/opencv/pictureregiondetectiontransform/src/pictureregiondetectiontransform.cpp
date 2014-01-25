#include "pictureregiondetectiontransform.h"

namespace media {

PictureRegionDetectionTransform::PictureRegionDetectionTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    }

//ElementBase::ParamList PictureRegionDetectionTransform::getParams() const
//    {
//    ParamList ret;
//    ret["File"] =  mPictureFrame.getSourceName();
//    return ret;
//    }

//void PictureRegionDetectionTransform::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    Q_UNUSED(aName);
//    mCascadeClassifier.load(std::string(qPrintable(aValue.toString())));
//    }

void PictureRegionDetectionTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (frame->getMaxDimension() == IplImageFrame::Dimensions)
                {
                mPictureFrame.setSourceName(frame->getSourceName());
                mPictureFrame.resizeAndCopyFrame(*frame);
                cv::Mat mat((IplImage*)mPictureFrame);
                mCascadeClassifier.detectMultiScale(mat, mRectVector, 1.1, 2, CV_HAAR_FIND_BIGGEST_OBJECT|CV_HAAR_DO_ROUGH_SEARCH|CV_HAAR_DO_CANNY_PRUNING|CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
                for (size_t i = 0; i < mRectVector.size(); ++i)
                    cvRectangleR((IplImage*)mPictureFrame, mRectVector[i], CV_RGB(255,255,255));

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
