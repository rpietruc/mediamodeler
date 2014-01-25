#include "picturedrawdestination.h"
#include <QDebug>
#include <QTimer>
#include <QImage>
#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include "pictureframes.h"
#include "drawwidget.h"

namespace media {

PictureDrawDestination::PictureDrawDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mTimer(new QTimer(this)),
    mWindow(new DrawWidget),
    mImage(NULL)
    {
    QObject::connect(mTimer, SIGNAL(timeout()), this, SLOT(showPicture()));
    mTimer->setInterval(25);
    }

PictureDrawDestination::~PictureDrawDestination()
    {
    mTimer->stop();

    delete mTimer;
    delete mImage;
    delete mWindow;
    }

//ElementBase::ParamList PictureDrawDestination::getParams() const
//    {
//    ParamList ret;
//    ret["Delay time [ms]"] = QVariant(mTimer->interval());
//    return ret;
//    }

//void PictureDrawDestination::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    Q_UNUSED(aName);
//    if (mTimer->interval() != aValue.toInt())
//        {
//        mTimer->stop();
//        mTimer->setInterval(aValue.toInt());
//        }
//    }

void PictureDrawDestination::showPicture()
    {
    if (mImage)
        mWindow->showPicture(*mImage);
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
                if ((frame->getDimension(IplImageFrame::Width).mResolution) &&
                    (frame->getDimension(IplImageFrame::Height).mResolution))
                    {
                    if (mImage &&
                      ((mImage->width() != frame->getDimension(IplImageFrame::Width).mResolution) ||
                       (mImage->height() != frame->getDimension(IplImageFrame::Height).mResolution)))
                        {
                        delete mImage;
                        mImage = NULL;
                        }

                    if (!mImage)
                        mImage = new QImage(frame->getDimension(IplImageFrame::Width).mResolution, frame->getDimension(IplImageFrame::Height).mResolution, QImage::Format_RGB32);

                    int point[IplImageFrame::Dimensions] = {0};
                    for (point[IplImageFrame::Height] = 0; point[IplImageFrame::Height] < frame->getDimension(IplImageFrame::Height).mResolution; ++point[IplImageFrame::Height])
                        for (point[IplImageFrame::Width] = 0; point[IplImageFrame::Width] < frame->getDimension(IplImageFrame::Width).mResolution; ++point[IplImageFrame::Width])
                            {
                            int rgb[3] = {0};
                            if (frame->getMaxDimension() == (IplImageFrame::Dimensions - 1))
                                rgb[0] = rgb[1] = rgb[2] = frame->getSample(point);
                            else // if (frame->getMaxDimension() == IplImageFrame::Dimensions)
                                for (point[IplImageFrame::Channels] = 0; point[IplImageFrame::Channels] < frame->getDimension(IplImageFrame::Channels).mResolution; ++point[IplImageFrame::Channels])
                                    rgb[point[IplImageFrame::Channels]] = frame->getSample(point);
                            mImage->setPixel(point[IplImageFrame::Width], point[IplImageFrame::Height], qRgb(rgb[2], rgb[1], rgb[0]));
                            }
                    mWindow->setWindowTitle(frame->getSourceName());

                    if (!mTimer->isActive())
                        mTimer->start();
                    }
                emit framesProcessed();
                }
            }
    }

} // namespace media
