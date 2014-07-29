#include "imageextractroadtransform.h"
#include <itkContourExtractor2DImageFilter.h>
#include <itkApproximateSignedDistanceMapImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <otbPolyLineParametricPathWithValue.h>
#include <otbRoadExtractionFilter.h>
#include <otbDrawPathListFilter.h>
#include <otbImage.h>
#include <otbVectorImage.h>
#include <itkRescaleIntensityImageFilter.h>
#include <otbMath.h>
#include <itkInvertIntensityImageFilter.h>
#include <itkGrayscaleDilateImageFilter.h>
#include <itkBinaryBallStructuringElement.h>

#include <QDebug>

//using namespace otb;
//using namespace itk;
using namespace std;

namespace media {

ImageExtractRoadTransform::ImageExtractRoadTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("referenceChannel0", 337);
    setProperty("referenceChannel1", 557);
    setProperty("referenceChannel2", 432);
    setProperty("referenceChannel3", 859);
    setProperty("alpha", 1.0);
    setProperty("amplitudeThreshold", 0.00005);
    setProperty("tolerance", 1.0);
    setProperty("maxAngle", (double)otb::CONST_PIl/8);
    setProperty("angularThreshold", (double)otb::CONST_PIl/8);
    setProperty("firstMeanDistanceThreshold", 1);
    setProperty("secondMeanDistanceThreshold", 10);
    setProperty("distanceThreshold", 25);
    }

void ImageExtractRoadTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            VectorOtbFrame srcVectorFrame;
            if (srcVectorFrame.isCopyable(*frame))
                {
                srcVectorFrame.setSourceName(frame->getSourceName());
                srcVectorFrame.resizeAndCopyFrame(*frame);

                typedef otb::PolyLineParametricPathWithValue<FloatOtbFrame::PixelType, FloatOtbFrame::Dimension> PathType;
                typedef otb::RoadExtractionFilter<VectorOtbFrame::ImageType, PathType> RoadExtractionFilterType;
                typedef otb::DrawPathListFilter<FloatOtbFrame::ImageType, PathType, FloatOtbFrame::ImageType> DrawPathFilterType;
                typedef itk::RescaleIntensityImageFilter<FloatOtbFrame::ImageType, ImageOtbFrame::ImageType> RescalerType;

                RoadExtractionFilterType::Pointer roadExtractionFilter = RoadExtractionFilterType::New();
                DrawPathFilterType::Pointer drawingFilter = DrawPathFilterType::New();
                RescalerType::Pointer rescaleFilter = RescalerType::New();

                VectorOtbFrame::ImageType::PixelType ReferencePixel;
                ReferencePixel.SetSize(4);
                ReferencePixel.SetElement(0, property("referenceChannel0").toDouble());
                ReferencePixel.SetElement(1, property("referenceChannel1").toDouble());
                ReferencePixel.SetElement(2, property("referenceChannel2").toDouble());
                ReferencePixel.SetElement(3, property("referenceChannel3").toDouble());

                roadExtractionFilter->SetReferencePixel(ReferencePixel);
                roadExtractionFilter->SetAlpha(property("alpha").toDouble());
                roadExtractionFilter->SetAmplitudeThreshold(property("amplitudeThreshold").toDouble());
                roadExtractionFilter->SetTolerance(property("tolerance").toDouble());
                roadExtractionFilter->SetMaxAngle(property("maxAngle").toDouble());
                roadExtractionFilter->SetAngularThreshold(property("angularThreshold").toDouble());
                roadExtractionFilter->SetFirstMeanDistanceThreshold(property("firstMeanDistanceThreshold").toDouble());
                roadExtractionFilter->SetSecondMeanDistanceThreshold(property("secondMeanDistanceThreshold").toDouble());
                roadExtractionFilter->SetDistanceThreshold(property("distanceThreshold").toDouble());

                FloatOtbFrame::ImageType::Pointer blackBackground = FloatOtbFrame::ImageType::New();
                blackBackground->SetRegions(srcVectorFrame.operator ImageType::Pointer()->GetLargestPossibleRegion());
                blackBackground->Allocate();
                blackBackground->FillBuffer(0);

                drawingFilter->UseInternalPathValueOn();
                rescaleFilter->SetOutputMinimum(itk::NumericTraits<ImageOtbFrame::PixelType>::min());
                rescaleFilter->SetOutputMaximum(itk::NumericTraits<ImageOtbFrame::PixelType>::max());

                roadExtractionFilter->SetInput(srcVectorFrame.operator ImageType::Pointer());
                drawingFilter->SetInput(blackBackground);
                drawingFilter->SetInputPath(roadExtractionFilter->GetOutput());
                rescaleFilter->SetInput(drawingFilter->GetOutput());

                try
                    {
                    rescaleFilter->Update();
                    }
                catch (itk::ExceptionObject & exp)
                    {
                    qWarning() << "ITK::Exception catched : " << exp.what();
                    }

                // output image enhancement
                typedef itk::BinaryBallStructuringElement<ImageOtbFrame::PixelType, ImageOtbFrame::Dimension> StructuringElementType;
                typedef itk::GrayscaleDilateImageFilter<ImageOtbFrame::ImageType, ImageOtbFrame::ImageType, StructuringElementType> DilateFilterType;
                typedef itk::InvertIntensityImageFilter<ImageOtbFrame::ImageType, ImageOtbFrame::ImageType> InvertFilterType;

                StructuringElementType se;
                se.SetRadius(1);
                se.CreateStructuringElement();

                DilateFilterType::Pointer dilater = DilateFilterType::New();

                dilater->SetInput(rescaleFilter->GetOutput());
                dilater->SetKernel(se);

                InvertFilterType::Pointer invertFilter = InvertFilterType::New();
                invertFilter->SetInput(dilater->GetOutput());

                mImageFrame.resizeAndCopyImage(invertFilter->GetOutput());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
