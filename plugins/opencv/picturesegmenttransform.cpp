#include "picturesegmenttransform.h"
#include "pictureframes.h"
#include <boost/shared_ptr.hpp>
#include "regiongrowingpcnn.h"

using namespace boost;

namespace media {

PictureSegmentTransform::PictureSegmentTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    // Default parameters
    setProperty("d", .5);
    setProperty("minBeta", 0.2);
    setProperty("deltaBeta", 0.01);
    setProperty("maxBeta", 1.);
    setProperty("SBmax", 0.2);
    }

void PictureSegmentTransform::process()
    {
    PictureGrayFrame srcFrame; // <-- RGB image
    mPictureFrame.release(); // <-- Gray/binary image

    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            PictureRGBFrame inputFrame;
            if (inputFrame.isCopyable(*frame))
                {
                inputFrame.resizeAndCopyFrame(*frame);
                if (inputFrame.getDimensions() == frame->getDimensions())
                    {
                    srcFrame.resizeAndCopyFrame(inputFrame);
                    mPictureFrame.setSourceName(frame->getSourceName());
                    if (mPictureFrame.isEmpty())
                        {
                        mPictureFrame.resize(srcFrame.getDimensionT(PictureRGBFrame::Width).mResolution, srcFrame.getDimensionT(PictureRGBFrame::Height).mResolution);
                        mPictureFrame.clear();
                        }
                    }
                else
                    mPictureFrame.resizeAndCopyFrame(inputFrame);
                }
            }

    if (!srcFrame.isEmpty())
        {
        regionGrowingPcnn(srcFrame, mPictureFrame, property("minBeta").toDouble(), property("maxBeta").toDouble(), property("deltaBeta").toDouble(), property("d").toDouble(), property("SBMax").toDouble());
        emit framesReady();
        }
    }
} // namespace media
