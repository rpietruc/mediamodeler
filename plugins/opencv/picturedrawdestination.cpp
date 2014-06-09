#include "picturedrawdestination.h"
#include <QDebug>
#include <QTimer>
#include <QImage>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include "pictureframes.h"
#include "drawwidget.h"
#include <QDynamicPropertyChangeEvent>

namespace media {

PictureDrawDestination::PictureDrawDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mTimer(new QTimer(this)),
    mWindow(new DrawWidget),
    mImage(NULL),
    mImageReady(false)
    {
    QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(showPicture()));
    setProperty("delayTime", DefaultDelay);
    }

PictureDrawDestination::~PictureDrawDestination()
    {
    delete mImage;
    delete mWindow;
    }

bool PictureDrawDestination::event(QEvent *aEvent)
    {
    if (aEvent->type() == QEvent::DynamicPropertyChange)
        {
        QDynamicPropertyChangeEvent *event = (QDynamicPropertyChangeEvent*)aEvent;
        if (QString(event->propertyName().constData()) == "delayTime")
            {
            int delayTime = DefaultDelay;
            if (property("delayTime").canConvert(QVariant::Int) && (property("delayTime").toInt() > 0))
                delayTime = property("delayTime").toInt();
            mTimer->start(delayTime);
            event->accept();
            return true;
            }
        }
    return ElementBase::event(aEvent);
    }

void PictureDrawDestination::showPicture()
    {
    if (mImageReady)
        {
        Q_ASSERT(mImage);
        mWindow->showPicture(*mImage);
        mImageReady = false;
        emit framesProcessed();
        }
    }

void PictureDrawDestination::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == IplImageFrame::Dimensions) ||
                (frame->getMaxDimension() == (IplImageFrame::Dimensions - 1)))
                {
                if ((frame->getDimensionT(IplImageFrame::Width).mResolution) &&
                    (frame->getDimensionT(IplImageFrame::Height).mResolution))
                    {
                    if (mImage &&
                      ((mImage->width() != frame->getDimensionT(IplImageFrame::Width).mResolution) ||
                       (mImage->height() != frame->getDimensionT(IplImageFrame::Height).mResolution)))
                        {
                        delete mImage;
                        mImage = NULL;
                        }

                    if (!mImage)
                        mImage = new QImage(frame->getDimensionT(IplImageFrame::Width).mResolution, frame->getDimensionT(IplImageFrame::Height).mResolution, QImage::Format_RGB32);

                    int point[IplImageFrame::Dimensions] = {0};
                    for (point[IplImageFrame::Height] = 0; point[IplImageFrame::Height] < frame->getDimensionT(IplImageFrame::Height).mResolution; ++point[IplImageFrame::Height])
                        for (point[IplImageFrame::Width] = 0; point[IplImageFrame::Width] < frame->getDimensionT(IplImageFrame::Width).mResolution; ++point[IplImageFrame::Width])
                            {
                            int rgb[3] = {0};
                            if (frame->getMaxDimension() == (IplImageFrame::Dimensions - 1))
                                rgb[0] = rgb[1] = rgb[2] = frame->getSampleT(point);
                            else // if (frame->getMaxDimension() == IplImageFrame::Dimensions)
                                for (point[IplImageFrame::Channels] = 0; point[IplImageFrame::Channels] < frame->getDimensionT(IplImageFrame::Channels).mResolution; ++point[IplImageFrame::Channels])
                                    rgb[point[IplImageFrame::Channels]] = frame->getSampleT(point);
                            mImage->setPixel(point[IplImageFrame::Width], point[IplImageFrame::Height], qRgb(rgb[2], rgb[1], rgb[0]));
                            }

                    mWindow->setWindowTitle(frame->getSourceName());
                    mImageReady = true;

                    //color frames have priority
                    if (frame->getMaxDimension() == IplImageFrame::Dimensions)
                        return;
                    }
                }
            }
    }

} // namespace media
