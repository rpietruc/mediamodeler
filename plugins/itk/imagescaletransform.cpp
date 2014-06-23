#include "imagescaletransform.h"
#include <itkScaleTransform.h>
#include <itkResampleImageFilter.h>

using namespace itk;

namespace media {

ImageScaleTransform::ImageScaleTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("widthScale", 1.5);
    setProperty("heightScale", 1.5);
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

                typedef ScaleTransform<double, 2> TransformType;
                TransformType::Pointer scaleTransform = TransformType::New();
                FixedArray<float, 2> scale;
                scale[0] = property("widthScale").toDouble();
                scale[1] = property("heightScale").toDouble();
                scaleTransform->SetScale(scale);
                Point<float, 2> center;
                center[0] = srcImg->GetLargestPossibleRegion().GetSize()[0]/2;
                center[1] = srcImg->GetLargestPossibleRegion().GetSize()[1]/2;
                scaleTransform->SetCenter(center);

                typedef ResampleImageFilter<ColorImageFrame::ImageType, ColorImageFrame::ImageType> ResampleImageFilterType;
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
