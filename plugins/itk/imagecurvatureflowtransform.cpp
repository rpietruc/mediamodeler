#include "imagecurvatureflowtransform.h"
#include <itkCurvatureFlowImageFilter.h>

using namespace itk;

namespace media {

ImageCurvatureFlowTransform::ImageCurvatureFlowTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("iterations", 5);
    setProperty("timeStep", 0.125);
    }

void ImageCurvatureFlowTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (mSrcFrame.isCopyable(*frame))
                {
                mImageFrame.clear();
                mImageFrame.setSourceName(frame->getSourceName());
                mSrcFrame.resizeAndCopyFrame(*frame);
                FloatImageFrame::ImageType::Pointer srcImg = mSrcFrame;

                typedef FloatImageFrame::PixelType InternalPixelType;
                const unsigned int Dimension = 2;
                typedef Image<InternalPixelType, Dimension> InternalImageType;

                typedef GrayImageFrame::PixelType OutputPixelType;
                typedef Image<OutputPixelType, Dimension> OutputImageType;

                typedef CurvatureFlowImageFilter<InternalImageType, InternalImageType> CurvatureFlowImageFilterType;
                CurvatureFlowImageFilterType::Pointer curvatureflow = CurvatureFlowImageFilterType::New();

                curvatureflow->SetNumberOfIterations(property("iterations").toInt());
                curvatureflow->SetTimeStep(property("timeStep").toDouble());

                curvatureflow->SetInput(srcImg);
                curvatureflow->Update();
                mImageFrame.resizeAndCopyImage(curvatureflow->GetOutput());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
