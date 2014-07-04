#include "picturefiledestination.h"
#include "pictureframes.h"
#include <QDebug>

namespace media {

PictureFileDestination::PictureFileDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mFileIndex(0)
    {
    setProperty("fileName", "output.png");
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
                QString fileName(property("fileName").toString());
                if (mFileIndex > 0)
                    fileName.replace(QString(".png"), QString(".%1.png").arg(mFileIndex));
                cvSaveImage(qPrintable(fileName), (IplImage*)rgbImg);
                ++mFileIndex;
                emit framesProcessed();
                }
            }
    }

} // namespace media
