#include "videocamerasource.h"

//most Logitech UVC webcams, can't resume correctly from USB suspend
//echo -1 > /sys/module/usbcore/parameters/autosuspend
//Grab and show the input of a video4linux2 device
//ffplay -f video4linux2 -framerate 30 -video_size hd720 /dev/video0
//Grab and record the input of a video4linux2 device, leave the frame rate and size as previously set
//ffmpeg -f video4linux2 -input_format mjpeg -i /dev/video0 out.mpeg

#define MAX_VIDEO_DEVICES 64

namespace media {

VideoCameraSource::VideoCameraSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mCapture(NULL)
    {
    open("/dev/video0");
    }

VideoCameraSource::~VideoCameraSource()
    {
    if (mCapture)
        cvReleaseCapture(&mCapture);
    }
ElementBase::ParamList VideoCameraSource::getParams()
    {
    ParamList ret;
    ret["File"] = mPictureFrame.getSourceName();
    return ret;
    }

void VideoCameraSource::setParamValue(const QString& aName, const QVariant& aValue)
    {
    Q_UNUSED(aName);
    if (mPictureFrame.getSourceName() != aValue.toString())
        open(aValue.toString());
    }

void VideoCameraSource::open(const QString &aFileName)
    {
    if (mCapture && (mPictureFrame.getSourceName() != aFileName))
        {
        cvReleaseCapture(&mCapture);
        mCapture = NULL;
        }

    if (!mCapture)
        {
        int camIndex = getCamIndex(aFileName);
        if (camIndex >= 0)
            {
            mCapture = cvCaptureFromCAM(camIndex);
            if (mCapture)
                {
                mPictureFrame.setSourceName(aFileName);
                mFrameIndex = 0;
                }
            }
        }
    }

void VideoCameraSource::process()
    {
    IplImage* img = NULL;
//    if (mFrameIndex < 5)
        img = cvQueryFrame(mCapture);
    if (img)
        {
        ++mFrameIndex;
        mPictureFrame.resizeAndCopyImage(*img);
        //cvReleaseImage(&img);
        emit framesReady();
        }
    else
        emit processingCompleted();
    }

int VideoCameraSource::getCamIndex(const QString &aFileName)
    {
    for (int i = 0; i < MAX_VIDEO_DEVICES; ++i)
        if (aFileName == QString("/dev/video%1").arg(i))
            return i;
    return -1;
    }

} // namespace media
