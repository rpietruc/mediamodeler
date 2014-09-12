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
    setProperty("d", 1.);
    setProperty("Omega", 1);
    setProperty("minBeta", 0.2);
    setProperty("deltaBeta", 0.01);
    setProperty("maxBeta", 1.);
    }

void PictureSegmentTransform::process()
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
                mPictureFrame.resize(srcFrame.getDimensionT(PictureRGBFrame::Width).mResolution, srcFrame.getDimensionT(PictureRGBFrame::Height).mResolution);

                shared_ptr<IplImage> grayImg(cvCreateImage(cvGetSize(srcFrame), 8, 1), IplImageDeleter());
                cvCvtColor(srcFrame, grayImg.get(), CV_BGR2GRAY);

                regionGrowingPcnn(grayImg.get(), mPictureFrame,
                                  property("Omega").toDouble(),
                                  property("minBeta").toDouble(),
                                  property("maxBeta").toDouble(),
                                  property("deltaBeta").toDouble(),
                                  property("d").toDouble());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
