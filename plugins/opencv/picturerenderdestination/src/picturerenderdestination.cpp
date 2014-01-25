#include "picturerenderdestination.h"
#include "pictureframes.h"
#include <QDebug>
#include <QTimer>

namespace media {

PictureRenderDestination::PictureRenderDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    mTimer = new QTimer(this);
    mTimer->setSingleShot(true);
    QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(showPicture()));
    setProperty("delayTime", 25);
    }

PictureRenderDestination::~PictureRenderDestination()
    {
    cvDestroyWindow(qPrintable(mPictureFrame.getSourceName()));
    }

void PictureRenderDestination::showPicture()
    {
    if (!mPictureFrame.getSourceName().isEmpty())
        {
        cvShowImage(qPrintable(mPictureFrame.getSourceName()), (IplImage*)mPictureFrame);
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
                mTimer->setInterval(property("delayTime").toInt());
                mTimer->start();
                }
            }
    }

} // namespace media
