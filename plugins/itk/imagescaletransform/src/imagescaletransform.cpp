#include "imagescaletransform.h"
#include "itkScaleTransform.h"
#include "itkResampleImageFilter.h"

using namespace itk;

namespace media {

ImageScaleTransform::ImageScaleTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    mScale[0] = 1.5;
    mScale[1] = 1.5;
    }

ElementBase::ParamList ImageScaleTransform::getParams() const
    {
    ParamList ret;
    ret["Width Scale"] =  mScale[0];
    ret["Height Scale"] = mScale[1];
    return ret;
    }

void ImageScaleTransform::setParamValue(const QString& aName, const QVariant& aValue)
    {
    if (aName == "Width Scale")
        mScale[0] = aValue.toDouble();
    else if (aName == "Height Scale")
        mScale[1] = aValue.toDouble();
    }

void ImageScaleTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (frame->getMaxDimension() == ColorImageFrame::Dimensions)
                {
                mImageFrame.setSourceName(frame->getSourceName());
                mSrcFrame.resizeAndCopyFrame(*frame);
                ColorImageFrame::ImageType::Pointer srcImg = mSrcFrame;

                typedef itk::ScaleTransform<double, 2> TransformType;
                TransformType::Pointer scaleTransform = TransformType::New();
                scaleTransform->SetScale(mScale);
                Point<float, 2> center;
                center[0] = srcImg->GetLargestPossibleRegion().GetSize()[0]/2;
                center[1] = srcImg->GetLargestPossibleRegion().GetSize()[1]/2;
                scaleTransform->SetCenter(center);

                typedef itk::ResampleImageFilter<ColorImageFrame::ImageType, ColorImageFrame::ImageType> ResampleImageFilterType;
                ResampleImageFilterType::Pointer resampleFilter = ResampleImageFilterType::New();
                resampleFilter->SetTransform(scaleTransform);
                resampleFilter->SetInput(srcImg);
                resampleFilter->SetSize(srcImg->GetLargestPossibleRegion().GetSize());
                resampleFilter->Update();
                mImageFrame.resizeAndCopyImage(resampleFilter->GetOutput());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
