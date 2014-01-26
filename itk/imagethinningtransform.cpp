#include "imagethinningtransform.h"
#include <itkBinaryThinningImageFilter.h>
#include <itkRescaleIntensityImageFilter.h>

using namespace itk;

namespace media {

ImageThinningTransform::ImageThinningTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    }

void ImageThinningTransform::process()
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

                // Rescale the image so that it can be seen (the output is 0 and 1, we want 0 and 255)
                typedef itk::BinaryThinningImageFilter<GrayImageFrame::ImageType, GrayImageFrame::ImageType> BinaryThinningImageFilterType;
                BinaryThinningImageFilterType::Pointer binaryThinningImageFilter = BinaryThinningImageFilterType::New();
                binaryThinningImageFilter->SetInput(srcImg);
                binaryThinningImageFilter->Update();

                // Rescale the image so that it can be seen (the output is 0 and 1, we want 0 and 255)
                typedef itk::RescaleIntensityImageFilter<GrayImageFrame::ImageType, GrayImageFrame::ImageType > RescaleFilterType;
                RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
                rescaler->SetInput(binaryThinningImageFilter->GetOutput());
                rescaler->SetOutputMinimum(0);
                rescaler->SetOutputMaximum(255);
                rescaler->Update();

                mImageFrame.resizeAndCopyImage(rescaler->GetOutput());
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
