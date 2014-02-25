#include "imageaddtransform.h"
#include <itkAddImageFilter.h>

using namespace itk;

namespace media {

ImageAddTransform::ImageAddTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    mSrcFrames.resize(2);
    }

void ImageAddTransform::process()
    {
    int index = 0;
    foreach (const ElementBase *source, mSourceElementsReadySet)
        {
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == ColorImageFrame::Dimensions) || (frame->getMaxDimension() == GrayImageFrame::Dimensions))
                {
                mImageFrame.setSourceName(frame->getSourceName());
                mSrcFrames[index++].resizeAndCopyFrame(*frame);
                break;
                }
            }
        if (index > 1)
            break;
        }

    typedef AddImageFilter<GrayImageFrame::ImageType, GrayImageFrame::ImageType> AddImageFilterType;
    AddImageFilterType::Pointer addFilter = AddImageFilterType::New();
    addFilter->SetInput1((GrayImageFrame::ImageType::Pointer)mSrcFrames[0]);
    addFilter->SetInput2((GrayImageFrame::ImageType::Pointer)mSrcFrames[1]);
    addFilter->Update();
    mImageFrame.resizeAndCopyImage(addFilter->GetOutput());

    emit framesReady();
    }

} // namespace media
