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

                //
                // Reference Pixel
                //
                VectorOtbFrame::ImageType::PixelType ReferencePixel;
                ReferencePixel.SetSize(4);
                ReferencePixel.SetElement(0, property("referenceChannel0").toDouble());
                ReferencePixel.SetElement(1, property("referenceChannel1").toDouble());
                ReferencePixel.SetElement(2, property("referenceChannel2").toDouble());
                ReferencePixel.SetElement(3, property("referenceChannel3").toDouble());

                //
                // Road Extraction
                //
                typedef otb::PolyLineParametricPathWithValue<FloatOtbFrame::PixelType, FloatOtbFrame::Dimension> PathType;
                typedef otb::RoadExtractionFilter<VectorOtbFrame::ImageType, PathType> RoadExtractionFilterType;

                RoadExtractionFilterType::Pointer roadExtractionFilter = RoadExtractionFilterType::New();
                roadExtractionFilter->SetReferencePixel(ReferencePixel);
                roadExtractionFilter->SetAlpha(property("alpha").toDouble());
                roadExtractionFilter->SetAmplitudeThreshold(property("amplitudeThreshold").toDouble());
                roadExtractionFilter->SetTolerance(property("tolerance").toDouble());
                roadExtractionFilter->SetMaxAngle(property("maxAngle").toDouble());
                roadExtractionFilter->SetAngularThreshold(property("angularThreshold").toDouble());
                roadExtractionFilter->SetFirstMeanDistanceThreshold(property("firstMeanDistanceThreshold").toDouble());
                roadExtractionFilter->SetSecondMeanDistanceThreshold(property("secondMeanDistanceThreshold").toDouble());
                roadExtractionFilter->SetDistanceThreshold(property("distanceThreshold").toDouble());
                roadExtractionFilter->SetInput(srcVectorFrame.operator ImageType::Pointer());
                try
                    {
                    roadExtractionFilter->Update();
                    }
                catch (itk::ExceptionObject & exp)
                    {
                    qWarning() << "ITK::Exception catched : " << exp.what();
                    }

                RoadExtractionFilterType::OutputPathListConstPointerType pathList = roadExtractionFilter->GetOutput();
                emit logMessage(Qt::blue, QString("found %1 paths").arg(pathList->Size()));

                RoadExtractionFilterType::OutputPathListType::ConstIterator pathIter = pathList->Begin();
                for (; pathIter != pathList->End(); ++pathIter)
                    {
                    vector< itk::Point<int> > points;
                    RoadExtractionFilterType::OutputPathType::VertexListConstIteratorType vertexIter = pathIter.Get()->GetVertexList()->Begin();
                    for (; vertexIter != pathIter.Get()->GetVertexList()->End(); ++vertexIter)
                        {
                        itk::Point<int> point;
                        point[0] = vertexIter->Value()[0];
                        point[1] = vertexIter->Value()[1];
                        points.push_back(point);
                        cout << vertexIter->Value() << ", " << vertexIter->Value()[0] << ": " << vertexIter->Value()[1] << endl;
                        }
                    PointsFrame pointsFrame;
                    pointsFrame.setPoints(points);
                    pointsFrame.setSourceName(frame->getSourceName());
                    mPointsFrameSet.push_back(pointsFrame);
                    }

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
