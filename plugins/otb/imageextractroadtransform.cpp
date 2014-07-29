#include "imageextractroadtransform.h"
#include <itkContourExtractor2DImageFilter.h>
#include <itkApproximateSignedDistanceMapImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <otbPolyLineParametricPathWithValue.h>
#include <otbRoadExtractionFilter.h>
#include <otbDrawPathListFilter.h>
#include <otbImage.h>
#include <otbVectorImage.h>
#include <otbImageFileReader.h>
#include <otbImageFileWriter.h>
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
    setProperty("fileName", "qb_RoadExtract.tif");
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
//    foreach (const ElementBase *source, mSourceElementsReadySet)
//        for (int i = 0; i < source-file:///home/rafal/devel/qmediamodeler/plugins/otb/imageextractroadtransform.cpp>getFramesNo(); ++i)
//            {
//            const FrameBase *frame = source->getFrame(i);
//            ColorImageFrame srcColorFrame;
//            if (srcColorFrame.isCopyable(*frame))
//                {
//                srcColorFrame.setSourceName(frame->getSourceName());
//                srcColorFrame.resizeAndCopyFrame(*frame);

                const unsigned int Dimension = 2;
                typedef double InputPixelType;
                typedef unsigned char OutputPixelType;
                typedef otb::VectorImage<InputPixelType, Dimension> InputVectorImageType;
                typedef otb::Image<InputPixelType, Dimension> InputImageType;
                typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
                typedef otb::PolyLineParametricPathWithValue<InputPixelType, Dimension> PathType;
                typedef otb::RoadExtractionFilter<InputVectorImageType, PathType> RoadExtractionFilterType;
                typedef otb::DrawPathListFilter<InputImageType, PathType, InputImageType> DrawPathFilterType;
                typedef itk::RescaleIntensityImageFilter<InputImageType, OutputImageType> RescalerType;
                typedef otb::ImageFileReader<InputVectorImageType> ReaderType;
                typedef otb::ImageFileWriter<OutputImageType> WriterType;

                ReaderType::Pointer reader = ReaderType::New();
                RoadExtractionFilterType::Pointer roadExtractionFilter = RoadExtractionFilterType::New();
                DrawPathFilterType::Pointer drawingFilter = DrawPathFilterType::New();
                RescalerType::Pointer rescaleFilter = RescalerType::New();
                WriterType::Pointer writer = WriterType::New();

                QString fileName = property("fileName").toString();
                reader->SetFileName(fileName.toStdString());
                reader->GenerateOutputInformation();

                InputVectorImageType::PixelType ReferencePixel;
                Q_ASSERT(reader->GetOutput()->GetNumberOfComponentsPerPixel() == 4);
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

                InputImageType::Pointer blackBackground = InputImageType::New();
                blackBackground->SetRegions(reader->GetOutput()->GetLargestPossibleRegion());
                //blackBackground->SetRegions(srcColorFrame.operator ImageType::Pointer()->GetLargestPossibleRegion());
                blackBackground->Allocate();
                blackBackground->FillBuffer(0);

                drawingFilter->UseInternalPathValueOn();
                rescaleFilter->SetOutputMinimum(itk::NumericTraits<OutputPixelType>::min());
                rescaleFilter->SetOutputMaximum(itk::NumericTraits<OutputPixelType>::max());

                roadExtractionFilter->SetInput(reader->GetOutput());
                //roadExtractionFilter->SetInput(srcColorFrame.operator ImageType::Pointer());
                drawingFilter->SetInput(blackBackground);
                drawingFilter->SetInputPath(roadExtractionFilter->GetOutput());
                rescaleFilter->SetInput(drawingFilter->GetOutput());
                rescaleFilter->Update();

                // output image enhancement
                typedef itk::BinaryBallStructuringElement<OutputPixelType, Dimension> StructuringElementType;
                typedef itk::GrayscaleDilateImageFilter<OutputImageType, OutputImageType, StructuringElementType> DilateFilterType;
                typedef itk::InvertIntensityImageFilter<OutputImageType, OutputImageType> InvertFilterType;

                StructuringElementType se;
                se.SetRadius(1);
                se.CreateStructuringElement();

                DilateFilterType::Pointer dilater = DilateFilterType::New();

                dilater->SetInput(rescaleFilter->GetOutput());
                dilater->SetKernel(se);

                InvertFilterType::Pointer invertFilter = InvertFilterType::New();
                invertFilter->SetInput(dilater->GetOutput());

                //mImageFrame = invertFilter->GetOutput();
                fileName += ".out.png";
                writer->SetFileName(fileName.toStdString());
                writer->SetInput(invertFilter->GetOutput());
                writer->Update();

                emit framesProcessed();
//                break;
//                }
//            }
    }

} // namespace media
