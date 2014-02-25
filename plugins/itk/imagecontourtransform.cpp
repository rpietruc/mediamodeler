#include "imagecontourtransform.h"
#include <itkContourExtractor2DImageFilter.h>
#include <itkApproximateSignedDistanceMapImageFilter.h>

using namespace itk;

namespace media {

ImageContourTransform::ImageContourTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    }

void ImageContourTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == ColorImageFrame::Dimensions) || (frame->getMaxDimension() == GrayImageFrame::Dimensions))
                {
//                mContourFrame.setSourceName(frame->getSourceName());
                mSrcFrame.resizeAndCopyFrame(*frame);

                typedef itk::Image<float, 2>  FloatImageType;
                typedef itk::ApproximateSignedDistanceMapImageFilter<GrayImageFrame::ImageType, FloatImageType> ApproximateSignedDistanceMapImageFilterType;
                ApproximateSignedDistanceMapImageFilterType::Pointer approximateSignedDistanceMapImageFilter = ApproximateSignedDistanceMapImageFilterType::New();
                approximateSignedDistanceMapImageFilter->SetInput((GrayImageFrame::ImageType::Pointer)mSrcFrame);

                //it seems that there can be only two regions

                approximateSignedDistanceMapImageFilter->SetInsideValue(255);
                approximateSignedDistanceMapImageFilter->SetOutsideValue(0);
                approximateSignedDistanceMapImageFilter->Update();
                typedef itk::ContourExtractor2DImageFilter<FloatImageType> ContourExtractor2DImageFilterType;
                ContourExtractor2DImageFilterType::Pointer contourExtractor2DImageFilter = ContourExtractor2DImageFilterType::New();
                contourExtractor2DImageFilter->SetInput(approximateSignedDistanceMapImageFilter->GetOutput());
                contourExtractor2DImageFilter->SetContourValue(0);
                contourExtractor2DImageFilter->Update();
                std::cout << "There are " << contourExtractor2DImageFilter->GetNumberOfOutputs() << " contours" << std::endl;
                for (unsigned int i = 0; i < contourExtractor2DImageFilter->GetNumberOfOutputs(); ++i)
                    {
                    std::cout << "Contour " << i << ": " << std::endl;
                    ContourExtractor2DImageFilterType::VertexListType::ConstIterator vertexIterator = contourExtractor2DImageFilter->GetOutput(i)->GetVertexList()->Begin();
                    while (vertexIterator != contourExtractor2DImageFilter->GetOutput(i)->GetVertexList()->End())
                        {
                        std::cout << vertexIterator->Value() << ", " << vertexIterator->Value()[0] << ": " << vertexIterator->Value()[1] << std::endl;
                        ++vertexIterator;
                        }
                    std::cout << std::endl;
                    }
//                mContourFrame.resizeAndCopyImage(contourExtractor2DImageFilter->GetOutput());


                //behave like destination by now, only for debuging purposes
                emit framesProcessed();

                //TODO: write result in some matrix, and
                //emit framesReady();
                break;
                }
            }
    }

} // namespace media
