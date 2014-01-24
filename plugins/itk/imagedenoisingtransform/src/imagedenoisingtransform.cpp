#include "imagedenoisingtransform.h"
#include <itkCurvatureFlowImageFilter.h>

using namespace itk;

namespace media {

ImageDenoisingTransform::ImageDenoisingTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mIterations(10),
    mTimeStep(0.25)
    {
    }

ElementBase::ParamList ImageDenoisingTransform::getParams() const
    {
    ParamList ret;
    ret["Iterations"] =  mIterations;
    ret["Time Step"] = mTimeStep;
    return ret;
    }

void ImageDenoisingTransform::setParamValue(const QString& aName, const QVariant& aValue)
    {
    if (aName == "Iterations")
        mIterations = aValue.toInt();
    else if (aName == "Time Step")
        mTimeStep = aValue.toDouble();
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
                denoisingFilter->SetNumberOfIterations(mIterations);
                denoisingFilter->SetTimeStep(mTimeStep);
                denoisingFilter->SetInput((GrayImageFrame::ImageType::Pointer)mSrcFrame);
                denoisingFilter->Update();
                mImageFrame.resizeAndCopyImage(denoisingFilter->GetOutput());
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
