#include "picturemorphologytransform.h"

namespace media {

PictureMorphologyTransform::PictureMorphologyTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("morphologySize", 1);
    setProperty("operation", CV_MOP_GRADIENT);
    mStructuringElement = cvCreateStructuringElementEx(property("morphologySize").toInt()*2 + 1, property("morphologySize").toInt()*2 + 1, property("morphologySize").toInt(), property("morphologySize").toInt(), CV_SHAPE_RECT, 0);
    }

PictureMorphologyTransform::~PictureMorphologyTransform()
    {
    cvReleaseStructuringElement(&mStructuringElement);
    }

void PictureMorphologyTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (mPictureFrame.isCopyable(*frame))
                {
                PictureRGBFrame srcFrame;
                srcFrame.resizeAndCopyFrame(*frame);

                mPictureFrame.setSourceName(frame->getSourceName());
                mPictureFrame.resize(srcFrame.getDimensionT(PictureRGBFrame::Width).mResolution,
                                     srcFrame.getDimensionT(PictureRGBFrame::Height).mResolution);

                Q_ASSERT(mStructuringElement);
                if (mStructuringElement->nCols != (property("morphologySize").toInt()*2 + 1))
                    {
                    cvReleaseStructuringElement(&mStructuringElement);
                    mStructuringElement = cvCreateStructuringElementEx(property("morphologySize").toInt()*2 + 1, property("morphologySize").toInt()*2 + 1, property("morphologySize").toInt(), property("morphologySize").toInt(), CV_SHAPE_RECT, 0);
                    }
                PictureRGBFrame tempFrame;
                tempFrame.resize(srcFrame.getDimensionT(PictureRGBFrame::Width).mResolution,
                                 srcFrame.getDimensionT(PictureRGBFrame::Height).mResolution);
                cvMorphologyEx(srcFrame, mPictureFrame, tempFrame, mStructuringElement, property("operation").toInt(), 1);

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
