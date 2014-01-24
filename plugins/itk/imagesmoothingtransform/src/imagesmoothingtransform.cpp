#include "imagesmoothingtransform.h"
#include <itkBilateralImageFilter.h>

using namespace itk;

namespace media {

ImageSmoothingTransform::ImageSmoothingTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mRangeSigma(50),
    mDomainSigma(5)
    {
    }

ElementBase::ParamList ImageSmoothingTransform::getParams() const
    {
    ParamList ret;
    ret["Range Sigma"] =  mRangeSigma;
    ret["Domain Sigma"] = mDomainSigma;
    return ret;
    }

void ImageSmoothingTransform::setParamValue(const QString& aName, const QVariant& aValue)
    {
    if (aName == "Range Sigma")
        mRangeSigma = aValue.toDouble();
    else if (aName == "Domain Sigma")
        mDomainSigma = aValue.toDouble();
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

                typedef itk::BilateralImageFilter<GrayImageFrame::ImageType, GrayImageFrame::ImageType> SmoothingImageFilterType;
                SmoothingImageFilterType::Pointer smoothingFilter = SmoothingImageFilterType::New();
                smoothingFilter->SetRangeSigma(mRangeSigma);
                smoothingFilter->SetDomainSigma(mDomainSigma);
                smoothingFilter->SetInput(srcImg);
                smoothingFilter->Update();
                mImageFrame.resizeAndCopyImage(smoothingFilter->GetOutput());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
