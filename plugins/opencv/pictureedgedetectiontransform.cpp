#include "pictureedgedetectiontransform.h"

namespace media {

PictureEdgeDetectionTransform::PictureEdgeDetectionTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
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
            if (mGrayImg.isCopyable(*frame))
                {
                PictureGrayFrame srcFrame;
                srcFrame.resizeAndCopyFrame(*frame);

                mGrayImg.setSourceName(frame->getSourceName());
                mGrayImg.resize(srcFrame.getDimensionT(PictureGrayFrame::Width).mResolution,
                                srcFrame.getDimensionT(PictureGrayFrame::Height).mResolution);

                cvCanny(srcFrame, mGrayImg,
                        property("threshold1").toDouble(),
                        property("threshold2").toDouble(),
                        property("apertureSize").toInt());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media

