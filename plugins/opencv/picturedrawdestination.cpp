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
            if (PictureRGBFrame().isCopyable(*frame))
                {
                if (mImage &&
                  ((mImage->width() != frame->getDimensionT(PictureRGBFrame::Width).mResolution) ||
                   (mImage->height() != frame->getDimensionT(PictureRGBFrame::Height).mResolution)))
                    {
                    delete mImage;
                    mImage = NULL;
                    }

                if (!mImage)
                    mImage = new QImage(frame->getDimensionT(PictureRGBFrame::Width).mResolution, frame->getDimensionT(PictureRGBFrame::Height).mResolution, QImage::Format_RGB32);

                int point[PictureRGBFrame::Dimensions] = {0};
                for (point[PictureRGBFrame::Height] = 0; point[PictureRGBFrame::Height] < frame->getDimensionT(PictureRGBFrame::Height).mResolution; ++point[PictureRGBFrame::Height])
                    for (point[PictureRGBFrame::Width] = 0; point[PictureRGBFrame::Width] < frame->getDimensionT(PictureRGBFrame::Width).mResolution; ++point[PictureRGBFrame::Width])
                        {
                        int rgb[3] = {0};
                        if (frame->getMaxDimension() == (PictureRGBFrame::Dimensions - 1))
                            rgb[0] = rgb[1] = rgb[2] = frame->getSampleT(point);
                        else // if (frame->getMaxDimension() == PictureRGBFrame::Dimensions)
                            for (point[PictureRGBFrame::Channels] = 0; point[PictureRGBFrame::Channels] < frame->getDimensionT(PictureRGBFrame::Channels).mResolution; ++point[PictureRGBFrame::Channels])
                                rgb[point[PictureRGBFrame::Channels]] = frame->getSampleT(point);
                        mImage->setPixel(point[PictureRGBFrame::Width], point[PictureRGBFrame::Height], qRgb(rgb[2], rgb[1], rgb[0]));
                        }

                mWindow->setWindowTitle(frame->getSourceName());
                mImageReady = true;

                //color frames have priority
                if (frame->getMaxDimension() == PictureRGBFrame::Dimensions)
                    return;
                }
            }
    }

} // namespace media
