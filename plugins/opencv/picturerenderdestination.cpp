#include "picturerenderdestination.h"
#include "pictureframes.h"
#include <QDebug>
#include <QTimer>
#include <QDynamicPropertyChangeEvent>

namespace media {

PictureRenderDestination::PictureRenderDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mTimer(new QTimer(this)),
    mImageReady(false)
    {
    QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(showPicture()));
    setProperty("delayTime", 0);
    }

PictureRenderDestination::~PictureRenderDestination()
    {
    cvDestroyWindow(qPrintable(mPictureFrame.getSourceName()));
    }

bool PictureRenderDestination::event(QEvent *aEvent)
    {
    if (aEvent->type() == QEvent::DynamicPropertyChange)
        {
        QDynamicPropertyChangeEvent *event = (QDynamicPropertyChangeEvent*)aEvent;
        if (QString(event->propertyName().constData()) == "delayTime")
            {
            mTimer->start(property("delayTime").toInt());
            event->accept();
            return true;
            }
        }
    return ElementBase::event(aEvent);
    }

void PictureRenderDestination::showPicture()
    {
    if (mImageReady)
        {
        if (!mPictureFrame.getSourceName().isEmpty())
            cvShowImage(qPrintable(mPictureFrame.getSourceName()), (IplImage*)mPictureFrame);
        mImageReady = false;
        emit framesProcessed();
        }
    }

void PictureRenderDestination::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == IplImageFrame::Dimensions) || (frame->getMaxDimension() == (IplImageFrame::Dimensions - 1)))
                {
                mPictureFrame.setSourceName(QString("%1::%2").arg(source->objectName()).arg(frame->getSourceName()));
                mPictureFrame.resizeAndCopyFrame(*frame);
                mImageReady = true;
                }
            }
    }

} // namespace media
