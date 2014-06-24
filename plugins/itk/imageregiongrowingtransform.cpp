#include "imageregiongrowingtransform.h"
#include <itkConfidenceConnectedImageFilter.h>
#include <itkCastImageFilter.h>

using namespace itk;

namespace media {

ImageRegionGrowingTransform::ImageRegionGrowingTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("multiplier", 2.5);
    setProperty("iterations", 5);
    setProperty("radius", 2);
    }

void ImageRegionGrowingTransform::process()
    {
    QVector< Point<int> > seedPoints;
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == PointsFrame::Dimensions) &&
                (frame->getDimensionT(PointsFrame::Axis).mResolution == PointsFrame::MaxAxis))
                {
                int point[PointsFrame::Dimensions] = {0, 0};
                for (point[PointsFrame::Index] = 0; point[PointsFrame::Index] < frame->getDimensionT(PointsFrame::Index).mResolution; ++point[PointsFrame::Index])
                    {
                    Point<int> seedPoint;
                    point[PointsFrame::Axis] = PointsFrame::XAxis;
                    seedPoint[0] = frame->getSampleT(point);
                    point[PointsFrame::Axis] = PointsFrame::YAxis;
                    seedPoint[1] = frame->getSampleT(point);
                    seedPoints.push_back(seedPoint);
                    }
                }
            else if ((frame->getMaxDimension() == ColorImageFrame::Dimensions) || (frame->getMaxDimension() == GrayImageFrame::Dimensions))
                {
                mSrcFrame.setSourceName(frame->getSourceName());
                mSrcFrame.resizeAndCopyFrame(*frame);
                }
            }

    mImageFrames.resize(seedPoints.size());
    for (int i = 0; i < seedPoints.size(); ++i)
        {
        FloatImageFrame::ImageType::Pointer srcImg = mSrcFrame;

        typedef FloatImageFrame::PixelType InternalPixelType;
        typedef Image<InternalPixelType, 2> InternalImageType;

        typedef GrayImageFrame::PixelType OutputPixelType;
        typedef Image<OutputPixelType, 2> OutputImageType;

        typedef ConfidenceConnectedImageFilter<InternalImageType, InternalImageType> ConnectedFilterType;
        ConnectedFilterType::Pointer confidenceConnected = ConnectedFilterType::New();

        typedef CastImageFilter<InternalImageType, OutputImageType> CastingFilterType;
        CastingFilterType::Pointer caster = CastingFilterType::New();

        confidenceConnected->SetInput(srcImg);
        caster->SetInput(confidenceConnected->GetOutput());

        confidenceConnected->SetMultiplier(property("multiplier").toInt());
        confidenceConnected->SetNumberOfIterations(property("iterations").toInt());
        confidenceConnected->SetReplaceValue(rand()%255);

        InternalImageType::IndexType index;
        index[0] = seedPoints.at(i)[0];
        index[1] = seedPoints.at(i)[1];
        emit logMessage(Qt::black, QString("point (%1, %2)").arg(index[0]).arg(index[1]));
        confidenceConnected->SetSeed(index);
        confidenceConnected->SetInitialNeighborhoodRadius(property("radius").toInt());

        confidenceConnected->Update();
        caster->Update();

//        mImageFrames[i].clear();
        mImageFrames[i].setSourceName(mSrcFrame.getSourceName());
        mImageFrames[i].resizeAndCopyImage(caster->GetOutput());
        emit framesReady();
        }
    }

} // namespace media
