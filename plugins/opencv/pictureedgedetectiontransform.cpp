#include "pictureedgedetectiontransform.h"

namespace media {

PictureEdgeDetectionTransform::PictureEdgeDetectionTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mGrayFrame(1),
    mPictureFrame(1)
    {
    setProperty("threshold1", 50);
    setProperty("threshold2", 100);
    setProperty("apertureSize", 5);
    }

void PictureEdgeDetectionTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (frame->getMaxDimension() == IplImageFrame::Dimensions)
                {
                mPictureFrame.setSourceName(frame->getSourceName());
                mSrcFrame.resizeAndCopyFrame(*frame);
                IplImage* srcImg = mSrcFrame;
                mGrayFrame.resizeAndCopyImage(*srcImg);
                mPictureFrame.resize(srcImg->width, srcImg->height);

                cvCanny(srcImg, mPictureFrame, property("threshold1").toDouble(), property("threshold2").toDouble(), property("apertureSize").toInt());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media

