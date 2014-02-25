#include "imagewatershedtransform.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkWatershedImageFilter.h"

using namespace itk;

namespace media {

ImageWatershedTransform::ImageWatershedTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("threshold", 0.005);
    setProperty("level", 0.5);
    }

void ImageWatershedTransform::process()
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

                typedef Image<float, 2> FloatImageType;
                typedef GradientMagnitudeImageFilter<GrayImageFrame::ImageType, FloatImageType> GradientMagnitudeImageFilterType;
                GradientMagnitudeImageFilterType::Pointer gradientMagnitudeImageFilter = GradientMagnitudeImageFilterType::New();
                gradientMagnitudeImageFilter->SetInput(srcImg);
                gradientMagnitudeImageFilter->Update();

                typedef WatershedImageFilter<FloatImageType> WatershedFilterType;
                WatershedFilterType::Pointer watershed = WatershedFilterType::New();
                watershed->SetThreshold(property("threshold").toDouble());
                watershed->SetLevel(property("level").toDouble());
                watershed->SetInput(gradientMagnitudeImageFilter->GetOutput());
                watershed->Update();

                typedef Image<unsigned long, 2> LabeledImageType;
                typedef ScalarToRGBColormapImageFilter<LabeledImageType, ColorImageFrame::ImageType> RGBFilterType;
                RGBFilterType::Pointer colormapImageFilter = RGBFilterType::New();
                colormapImageFilter->SetInput(watershed->GetOutput());
                colormapImageFilter->SetColormap(RGBFilterType::Jet);
                colormapImageFilter->Update();

                mImageFrame.resizeAndCopyImage(colormapImageFilter->GetOutput());
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
