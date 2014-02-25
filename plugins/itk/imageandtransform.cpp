#include "imageandtransform.h"
#include <itkAndImageFilter.h>

using namespace itk;

namespace media {

ImageAndTransform::ImageAndTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    mSrcFrames.resize(2);
    }

void ImageAndTransform::process()
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

    typedef AndImageFilter<GrayImageFrame::ImageType, GrayImageFrame::ImageType> AndImageFilterType;
    AndImageFilterType::Pointer andFilter = AndImageFilterType::New();
    andFilter->SetInput1((GrayImageFrame::ImageType::Pointer)mSrcFrames[0]);
    andFilter->SetInput2((GrayImageFrame::ImageType::Pointer)mSrcFrames[1]);
    andFilter->Update();
    mImageFrame.resizeAndCopyImage(andFilter->GetOutput());

    emit framesReady();
    }

} // namespace media
