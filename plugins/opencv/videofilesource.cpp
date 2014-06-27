#include "videofilesource.h"
#include <QDynamicPropertyChangeEvent>

namespace media {

VideoFileSource::VideoFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mCapture(NULL)
    {
    setProperty("fileName", QString());
    }

VideoFileSource::~VideoFileSource()
    {
    if (mCapture)
        cvReleaseCapture(&mCapture);
    }

bool VideoFileSource::event(QEvent *aEvent)
    {
    if (aEvent->type() == QEvent::DynamicPropertyChange)
        {
        QDynamicPropertyChangeEvent *event = (QDynamicPropertyChangeEvent*)aEvent;
        if (QString(event->propertyName().constData()) == "fileName")
            {
            open();
            event->accept();
            return true;
            }
        }
    return ElementBase::event(aEvent);
    }

void VideoFileSource::open()
    {
    if (mPictureFrame.getSourceName() != property("fileName").toString())
        {
        if (mCapture && mPictureFrame.getSourceName() != property("fileName").toString())
            {
            cvReleaseCapture(&mCapture);
            mCapture = NULL;
            }

        if (!mCapture)
            {
            mCapture = cvCaptureFromAVI(qPrintable(property("fileName").toString()));
            if (mCapture)
                mPictureFrame.setSourceName(property("fileName").toString());
            }
        }
    }

void VideoFileSource::process()
    {
    Q_ASSERT(mCapture);
    IplImage* img = cvQueryFrame(mCapture);
    if (img)
        {
        mPictureFrame = *img;
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
