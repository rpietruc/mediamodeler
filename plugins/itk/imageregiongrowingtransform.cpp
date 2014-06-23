#include "imageregiongrowingtransform.h"
#include <itkConfidenceConnectedImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkCurvatureFlowImageFilter.h>

using namespace itk;
using namespace std;

namespace media {

ImageRegionGrowingTransform::ImageRegionGrowingTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("seedX", 0);
    setProperty("seedY", 0);
    }

void ImageRegionGrowingTransform::process()
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

                typedef GrayImageFrame::PixelType InternalPixelType;
                const unsigned int Dimension = 2;
                typedef Image<InternalPixelType, Dimension> InternalImageType;

                typedef GrayImageFrame::PixelType OutputPixelType;
                typedef Image<OutputPixelType, Dimension> OutputImageType;

                typedef CastImageFilter<InternalImageType, OutputImageType> CastingFilterType;
                CastingFilterType::Pointer caster = CastingFilterType::New();

                typedef CurvatureFlowImageFilter<InternalImageType, InternalImageType> CurvatureFlowImageFilterType;
                CurvatureFlowImageFilterType::Pointer smoothing = CurvatureFlowImageFilterType::New();

                typedef ConfidenceConnectedImageFilter<InternalImageType, InternalImageType> ConnectedFilterType;
                ConnectedFilterType::Pointer confidenceConnected = ConnectedFilterType::New();

                smoothing->SetInput(srcImg);
                confidenceConnected->SetInput(smoothing->GetOutput());
                caster->SetInput(confidenceConnected->GetOutput());

                smoothing->SetNumberOfIterations(5);
                smoothing->SetTimeStep(0.125);
                confidenceConnected->SetMultiplier(2.5);
                confidenceConnected->SetNumberOfIterations(5);
                confidenceConnected->SetReplaceValue(255);

                InternalImageType::IndexType index;
                index[0] = property("seedX").toInt();
                index[1] = property("seedY").toInt();
                confidenceConnected->SetSeed(index);
                confidenceConnected->SetInitialNeighborhoodRadius(2);

                try
                    {
                    confidenceConnected->Update();
                    }
                catch (ExceptionObject& excep)
                    {
                    cerr << "Exception caught !" << endl;
                    cerr << excep << endl;
                    }
                mImageFrame.resizeAndCopyImage(caster->GetOutput());
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
