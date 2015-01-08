#include "picturecolorsegmenttransform.h"
#include "pictureframes.h"
#include <boost/shared_ptr.hpp>
#include "multiplechannelpcnn.h"

using namespace boost;
using namespace cv;
using namespace std;
using namespace mpcnn;

namespace media {

PictureColorSegmentTransform::PictureColorSegmentTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("threshold", 1.0);
    }

void PictureColorSegmentTransform::process()
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

                // For all PCNN neurons, we first determine the seeded neurons using getSeededPixels
                mPictureFrame = getSeededPixels(cv::Mat((IplImage *)srcFrame, true), property("threshold").toDouble());

                // The maximum value in getSeededPixels can then be quantified by the timing of the latest output pulse of the current RBF.
                vector<Mat> rbfs = getRBFOutput(cv::Mat((IplImage *)srcFrame, true), 1, property("threshold").toDouble());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
