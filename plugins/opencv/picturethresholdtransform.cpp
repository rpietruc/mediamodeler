#include "picturethresholdtransform.h"

namespace media {

PictureThresholdTransform::PictureThresholdTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mSrcFrame(1),
    mPictureFrame(1)
    {
    setProperty("threshold", 127);
    setProperty("maxValue", 255);
    setProperty("type", CV_THRESH_BINARY);
    }

void PictureThresholdTransform::process()
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
                mPictureFrame.resize(srcImg->width, srcImg->height, srcImg->nChannels);

                cvThreshold(srcImg, mPictureFrame, property("threshold").toDouble(), property("maxValue").toDouble(), property("type").toInt());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
