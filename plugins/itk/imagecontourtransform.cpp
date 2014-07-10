#include "imagecontourtransform.h"
#include <itkContourExtractor2DImageFilter.h>
#include <itkApproximateSignedDistanceMapImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <QSet>
#include <QDebug>

using namespace itk;
using namespace std;

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
                GrayImageFrame srcFrame;
                srcFrame.setSourceName(frame->getSourceName());
                srcFrame.resizeAndCopyFrame(*frame);

                QSet<int> regions;
                int point[GrayImageFrame::Dimensions];
                for (point[GrayImageFrame::Height] = 0; point[GrayImageFrame::Height] < srcFrame.getDimensionT(GrayImageFrame::Height).mResolution; ++point[GrayImageFrame::Height])
                    for (point[GrayImageFrame::Width] = 0; point[GrayImageFrame::Width] < srcFrame.getDimensionT(GrayImageFrame::Width).mResolution; ++point[GrayImageFrame::Width])
                        regions.insert(srcFrame.getSampleT(point));

                mPointsFrameSet.clear();

//                int randPoint[GrayImageFrame::Dimensions];
//                randPoint[GrayImageFrame::Width] = qrand()%srcFrame.getDimensionT(GrayImageFrame::Width).mResolution;
//                randPoint[GrayImageFrame::Height] = qrand()%srcFrame.getDimensionT(GrayImageFrame::Height).mResolution;
//                int i = srcFrame.getSampleT(randPoint);
//                int i = *(regions.begin() + qrand()%regions.size());
                foreach (int i, regions)
                    {
                    try {
                        typedef itk::Image<unsigned char, 2>  ImageType;
                        typedef itk::BinaryThresholdImageFilter <ImageType, ImageType> BinaryThresholdImageFilterType;
                        BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
                        thresholdFilter->SetInput((GrayImageFrame::ImageType::Pointer)srcFrame);
                        thresholdFilter->SetLowerThreshold(i);
                        thresholdFilter->SetUpperThreshold(i);
                        thresholdFilter->SetInsideValue(255);
                        thresholdFilter->SetOutsideValue(0);

                        typedef Image<float, 2>  FloatImageType;
                        typedef ApproximateSignedDistanceMapImageFilter<GrayImageFrame::ImageType, FloatImageType> ApproximateSignedDistanceMapImageFilterType;
                        ApproximateSignedDistanceMapImageFilterType::Pointer approximateSignedDistanceMapImageFilter = ApproximateSignedDistanceMapImageFilterType::New();
                        approximateSignedDistanceMapImageFilter->SetInput(thresholdFilter->GetOutput());

                        approximateSignedDistanceMapImageFilter->SetInsideValue(255);
                        approximateSignedDistanceMapImageFilter->SetOutsideValue(0);
                        approximateSignedDistanceMapImageFilter->Update();
                        typedef ContourExtractor2DImageFilter<FloatImageType> ContourExtractor2DImageFilterType;
                        ContourExtractor2DImageFilterType::Pointer contourExtractor2DImageFilter = ContourExtractor2DImageFilterType::New();
                        contourExtractor2DImageFilter->SetInput(approximateSignedDistanceMapImageFilter->GetOutput());
                        contourExtractor2DImageFilter->SetContourValue(0);
                        contourExtractor2DImageFilter->Update();

                        emit logMessage(Qt::blue, QString("region %1 with %2 contours").arg(i).arg(contourExtractor2DImageFilter->GetNumberOfOutputs()));
//                        cout << i << " there are " << contourExtractor2DImageFilter->GetNumberOfOutputs() << " contours" << endl;
                        for (unsigned int i = 0; i < contourExtractor2DImageFilter->GetNumberOfOutputs(); ++i)
                            {
                            vector< Point<int> > points;
//                            cout << "Contour " << i << ": " << endl;
                            ContourExtractor2DImageFilterType::VertexListType::ConstIterator vertexIterator = contourExtractor2DImageFilter->GetOutput(i)->GetVertexList()->Begin();
                            while (vertexIterator != contourExtractor2DImageFilter->GetOutput(i)->GetVertexList()->End())
                                {
                                Point<int> point;
                                point[0] = vertexIterator->Value()[0];
                                point[1] = vertexIterator->Value()[1];
                                points.push_back(point);
//                                cout << vertexIterator->Value() << ", " << vertexIterator->Value()[0] << ": " << vertexIterator->Value()[1] << endl;
                                ++vertexIterator;
                                }
                            PointsFrame pointsFrame;
                            pointsFrame.setPoints(points);
                            pointsFrame.setSourceName(frame->getSourceName());
                            mPointsFrameSet.push_back(pointsFrame);
//                            cout << endl;
                            }
                        }
                    catch (...)
                        {
                        qWarning() << "exception catched - ignored.";
                        }
                    }
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
