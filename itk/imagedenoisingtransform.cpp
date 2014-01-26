#include "imagedenoisingtransform.h"
#include <itkCurvatureFlowImageFilter.h>

using namespace itk;

namespace media {

ImageDenoisingTransform::ImageDenoisingTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("iterations", 10);
    setProperty("timeStep", 0.25);
    }

void ImageDenoisingTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == ColorImageFrame::Dimensions) || (frame->getMaxDimension() == GrayImageFrame::Dimensions))
                {
                mImageFrame.setSourceName(frame->getSourceName());
                mSrcFrame.resizeAndCopyFrame(*frame);
                typedef itk::CurvatureFlowImageFilter<GrayImageFrame::ImageType, GrayImageFrame::ImageType> DenoisingImageFilterType;
                DenoisingImageFilterType::Pointer denoisingFilter = DenoisingImageFilterType::New();
                denoisingFilter->SetNumberOfIterations(property("iterations").toInt());
                denoisingFilter->SetTimeStep(property("timeStep").toInt());
                denoisingFilter->SetInput((GrayImageFrame::ImageType::Pointer)mSrcFrame);
                denoisingFilter->Update();
                mImageFrame.resizeAndCopyImage(denoisingFilter->GetOutput());
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
