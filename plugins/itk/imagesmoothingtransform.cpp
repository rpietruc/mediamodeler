#include "imagesmoothingtransform.h"
#include <itkBilateralImageFilter.h>

using namespace itk;

namespace media {

ImageSmoothingTransform::ImageSmoothingTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("rangeSigma", 50);
    setProperty("domainSigma", 5);
    }

void ImageSmoothingTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == ColorImageFrame::Dimensions) || (frame->getMaxDimension() == GrayImageFrame::Dimensions))
                {
                mImageFrame.setSourceName(frame->getSourceName());
                mSrcFrame.resizeAndCopyFrame(*frame);
                GrayImageFrame::ImageType::Pointer srcImg = mSrcFrame;

                typedef BilateralImageFilter<GrayImageFrame::ImageType, GrayImageFrame::ImageType> SmoothingImageFilterType;
                SmoothingImageFilterType::Pointer smoothingFilter = SmoothingImageFilterType::New();
                smoothingFilter->SetRangeSigma(property("rangeSigma").toDouble());
                smoothingFilter->SetDomainSigma(property("domainSigma").toDouble());
                smoothingFilter->SetInput(srcImg);
                smoothingFilter->Update();
                mImageFrame.resizeAndCopyImage(smoothingFilter->GetOutput());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
