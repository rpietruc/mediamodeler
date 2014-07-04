#include "picturethresholdtransform.h"

namespace media {

PictureThresholdTransform::PictureThresholdTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
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
            if (mPictureFrame.isCopyable(*frame))
                {
                PictureGrayFrame srcFrame;
                srcFrame.resizeAndCopyFrame(*frame);

                mPictureFrame.setSourceName(frame->getSourceName());
                mPictureFrame.resize(srcFrame.getDimensionT(PictureGrayFrame::Width).mResolution,
                                     srcFrame.getDimensionT(PictureGrayFrame::Height).mResolution);

                cvThreshold(srcFrame, mPictureFrame,
                            property("threshold").toDouble(),
                            property("maxValue").toDouble(),
                            property("type").toInt());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
