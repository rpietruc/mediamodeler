#include "picturefiledestination.h"
#include "pictureframes.h"
#include <QDebug>

namespace media {

PictureFileDestination::PictureFileDestination(ElementFactory *aFactory, const QString &aObjectName) :
    FileDestination(aFactory, aObjectName)
    {
    }

void PictureFileDestination::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            PictureRGBFrame rgbImg;
            if (rgbImg.isCopyable(*frame))
                {
                rgbImg.resizeAndCopyFrame(*frame);
                cvSaveImage(qPrintable(getNextFilePath()), (IplImage*)rgbImg);
                emit framesProcessed();
                }
            }
    }

} // namespace media
