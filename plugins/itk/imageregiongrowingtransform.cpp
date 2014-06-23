#include "imageregiongrowingtransform.h"
#include <itkConfidenceConnectedImageFilter.h>
#include <itkCastImageFilter.h>

using namespace itk;

namespace media {

ImageRegionGrowingTransform::ImageRegionGrowingTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("seedX", 0);
    setProperty("seedY", 0);
    setProperty("multiplier", 2.5);
    setProperty("iterations", 5);
    setProperty("radius", 2);
    }

void ImageRegionGrowingTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == ColorImageFrame::Dimensions) || (frame->getMaxDimension() == GrayImageFrame::Dimensions))
                {
                mImageFrame.clear();
                mImageFrame.setSourceName(frame->getSourceName());
                mSrcFrame.resizeAndCopyFrame(*frame);
                FloatImageFrame::ImageType::Pointer srcImg = mSrcFrame;

                typedef FloatImageFrame::PixelType InternalPixelType;
                const unsigned int Dimension = 2;
                typedef Image<InternalPixelType, Dimension> InternalImageType;

                typedef GrayImageFrame::PixelType OutputPixelType;
                typedef Image<OutputPixelType, Dimension> OutputImageType;

                typedef ConfidenceConnectedImageFilter<InternalImageType, InternalImageType> ConnectedFilterType;
                ConnectedFilterType::Pointer confidenceConnected = ConnectedFilterType::New();

                typedef CastImageFilter<InternalImageType, OutputImageType> CastingFilterType;
                CastingFilterType::Pointer caster = CastingFilterType::New();

                confidenceConnected->SetInput(srcImg);
                caster->SetInput(confidenceConnected->GetOutput());

                confidenceConnected->SetMultiplier(property("multiplier").toInt());
                confidenceConnected->SetNumberOfIterations(property("iterations").toInt());
                confidenceConnected->SetReplaceValue(255);

                InternalImageType::IndexType index;
                index[0] = property("seedX").toInt();
                index[1] = property("seedY").toInt();
                confidenceConnected->SetSeed(index);
                confidenceConnected->SetInitialNeighborhoodRadius(property("radius").toInt());

                confidenceConnected->Update();
                caster->Update();

                mImageFrame.resizeAndCopyImage(caster->GetOutput());
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
