#include "picturesmoothingtransform.h"

namespace media {

PictureSmoothingTransform::PictureSmoothingTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("spatialRadius", 2);
    setProperty("colorRadius", 40);
    }

void PictureSmoothingTransform::process()
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
                mPictureFrame.resize(srcImg->width, srcImg->height);

                cvPyrMeanShiftFiltering(srcImg, mPictureFrame, property("spatialRadius").toDouble(), property("colorRadius").toDouble());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media

