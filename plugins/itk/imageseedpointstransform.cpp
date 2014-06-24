#include "imageseedpointstransform.h"

using namespace itk;
using namespace std;

namespace media {

ImageSeedPointsTransform::ImageSeedPointsTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    }

vector< Point<int> > ImageSeedPointsTransform::getGridPoints(const GrayImageFrame::ImageType::SizeType& aSize, const Point<int>& aResolution)
    {
    vector< Point<int> > points;
    for (int w = 0; w < aResolution[0]; ++w)
        for (int h = 0; h < aResolution[1]; ++h)
            {
            Point<int> point;
            point[0] = (w + 1)*(double)aSize[0]/(double)(aResolution[0] + 1);
            point[1] = (h + 1)*(double)aSize[1]/(double)(aResolution[1] + 1);
            emit logMessage(Qt::black, QString("point (%1, %2)").arg(point[0]).arg(point[1]));
            points.push_back(point);
            }
    return points;
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
                GrayImageFrame::ImageType::Pointer image = mSrcFrame;
                GrayImageFrame::ImageType::RegionType region = image->GetLargestPossibleRegion();
                GrayImageFrame::ImageType::SizeType size = region.GetSize();

                Point<int> resolution;
                resolution[0] = resolution[1] = 2;
                mPointsFrame = getGridPoints(size, resolution);
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
