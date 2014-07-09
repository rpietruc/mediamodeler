#include "picturesmoothingtransform.h"

namespace media {

PictureSmoothingTransform::PictureSmoothingTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("spatialRadius", 20);
    setProperty("colorRadius", 40);
    }

void PictureSmoothingTransform::process()
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

                cvPyrMeanShiftFiltering(srcFrame, mPictureFrame,
                                        property("spatialRadius").toDouble(),
                                        property("colorRadius").toDouble());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
