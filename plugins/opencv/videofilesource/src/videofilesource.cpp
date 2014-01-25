#include "videofilesource.h"

namespace media {

VideoFileSource::VideoFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mCapture(NULL)
    {
    }

VideoFileSource::~VideoFileSource()
    {
    if (mCapture)
        cvReleaseCapture(&mCapture);
    }

//ElementBase::ParamList VideoFileSource::getParams() const
//    {
//    ParamList ret;
//    ret["File"] = mPictureFrame.getSourceName();
//    return ret;
//    }

//void VideoFileSource::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    Q_UNUSED(aName);
//    if (mPictureFrame.getSourceName() != aValue.toString())
//        {
//        if (mCapture && mPictureFrame.getSourceName() != aValue.toString())
//            {
//            cvReleaseCapture(&mCapture);
//            mCapture = NULL;
//            }

//        if (!mCapture)
//            {
//            mCapture = cvCaptureFromAVI(qPrintable(aValue.toString()));
//            if (mCapture)
//                mPictureFrame.setSourceName(aValue.toString());
//            }
//        }
//    }

void VideoFileSource::process()
    {
    Q_ASSERT(mCapture);
    IplImage* img = cvQueryFrame(mCapture);
    if (img)
        {
        mPictureFrame.resizeAndCopyImage(*img);
        //cvReleaseImage(&img);
        emit framesReady();
        }
    else
        {
        cvReleaseCapture(&mCapture);
        mCapture = NULL;
        emit processingCompleted();
        }
    }

} // namespace media
