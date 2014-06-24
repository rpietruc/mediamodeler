#include "imageaddtransform.h"
#include <itkAddImageFilter.h>

using namespace itk;

namespace media {

ImageAddTransform::ImageAddTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    }

void ImageAddTransform::process()
    {
    int first = true;
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == ColorImageFrame::Dimensions) || (frame->getMaxDimension() == GrayImageFrame::Dimensions))
                {
                mImageFrame.setSourceName(frame->getSourceName());
                if (first)
                    {
                    mImageFrame.resizeAndCopyFrame(*frame);
                    first = false;
                    }
                else
                    {
                    GrayImageFrame srcFrame;
                    srcFrame.resizeAndCopyFrame(*frame);

                    typedef AddImageFilter<GrayImageFrame::ImageType, GrayImageFrame::ImageType> AddImageFilterType;
                    AddImageFilterType::Pointer addFilter = AddImageFilterType::New();
                    addFilter->SetInput1((GrayImageFrame::ImageType::Pointer)mImageFrame);
                    addFilter->SetInput2((GrayImageFrame::ImageType::Pointer)srcFrame);
                    addFilter->Update();
                    mImageFrame.resizeAndCopyImage(addFilter->GetOutput());
//                    mImageFrame.resizeAndCopyImage(srcFrame);
                    }
                }
            }
    emit framesReady();
    }

} // namespace media
