#include "imageseedpointstransform.h"

using namespace itk;
using namespace std;

namespace media {

ImageSeedPointsTransform::ImageSeedPointsTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    }

void ImageSeedPointsTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == ColorImageFrame::Dimensions) || (frame->getMaxDimension() == GrayImageFrame::Dimensions))
                {
                mSrcFrame.resizeAndCopyFrame(*frame);

                Point<int> point;
                point[0] = mSrcFrame.getDimensionT(GrayImageFrame::Width).mResolution/2;
                point[1] = mSrcFrame.getDimensionT(GrayImageFrame::Height).mResolution/2;
                vector< Point<int> > points;
                points.push_back(point);
                mPointsFrame.operator =(points);

                emit logMessage(Qt::black, QString("point (%1, %2)").arg(point[0]).arg(point[1]));

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
