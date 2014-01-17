#include "pictureapproximationtransform.h"
#include "segmentcontour.h"
#include "ellipsecontour.h"
#include "linecontour.h"

using namespace cv;
using namespace std;

namespace media {

bool compareFit(ContourApprox* a1, ContourApprox* a2)
    {
    bool ret = true;
    if (a1->isApproximated() && a2->isApproximated())
        {
        double fit1 = matchShapes(a1->getApproxPoints(), a1->getContourPoints(), CV_CONTOURS_MATCH_I1, 0);
        double fit2 = matchShapes(a2->getApproxPoints(), a2->getContourPoints(), CV_CONTOURS_MATCH_I1, 0);
        ret =  (fit1 < fit2);
        }
    else if (!a1->isApproximated())
        ret = false;

    return ret;
    }

PictureApproximationTransform::PictureApproximationTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mMinArea(10),
    mAccuracy(0.04)
    {
    }

ElementBase::ParamList PictureApproximationTransform::getParams()
    {
    ParamList ret;
    ret["Min Area"] =  mMinArea;
    ret["Approx Accuracy"] = mAccuracy;
    return ret;
    }

void PictureApproximationTransform::setParamValue(const QString& aName, const QVariant& aValue)
    {
    if (aName == "Min Area")
        mMinArea = aValue.toDouble();
    else if (aName == "Approx Accuracy")
        mAccuracy = aValue.toDouble();
    }

void PictureApproximationTransform::process()
    {
    vector<vector<Point> > contours;
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (frame->getMaxDimension() == IplImageFrame::Dimensions)
                {
                mPictureFrame.resizeAndCopyFrame(*frame);
                mPictureFrame.clear();
                mPictureFrame.setSourceName(frame->getSourceName());
                }
            if ((frame->getMaxDimension() == PointsFrame::Dimensions) &&
                (frame->getDimension(PointsFrame::Axis).mResolution == PointsFrame::MaxAxis))
                {
                vector<Point> contour;
                contour.resize(frame->getDimension(PointsFrame::Index).mResolution);
                int point[PointsFrame::Dimensions] = {0, 0};
                for (point[PointsFrame::Index] = 0; point[PointsFrame::Index] < frame->getDimension(PointsFrame::Index).mResolution; ++point[PointsFrame::Index])
                    {
                    point[PointsFrame::Axis] = PointsFrame::XAxis;
                    contour[point[PointsFrame::Index]].x = frame->getSample(point);
                    point[PointsFrame::Axis] = PointsFrame::YAxis;
                    contour[point[PointsFrame::Index]].y = frame->getSample(point);
                    }
                contours.push_back(contour);
                }
            }

    for (size_t i = 0; i < contours.size(); ++i)
        {
        if (fabs(contourArea(Mat(contours[i]))) >= mMinArea)
            {
            ContourPoints contourPoints(contours[i]);
            SegmentContour segment(contourPoints, mAccuracy);
            for (size_t j = 0; j < segment.getSegments().size(); ++j)
                {
                vector<Point> points = segment.getSegments()[j];

                ContourApprox* fits[] = {new EllipseContour(points), new LineContour(points)};
                ContourApprox* bestFit = *min_element(fits, fits + sizeof(fits)/sizeof(fits[0]), compareFit);
                std::vector<cv::Point> approx = bestFit->getApproxPoints();

                CvScalar color = {theRNG().uniform(0.0, 255.0), theRNG().uniform(0.0, 255.0), theRNG().uniform(0.0, 255.0), theRNG().uniform(0.0, 255.0)};
//                cvLine(mPictureFrame, cvPoint(points[0].x, points[0].y), cvPoint(points[points.size() - 1].x, points[points.size() - 1].y), color);
                for (int k = 0; k < ((int)approx.size() - 1); ++k)
                    cvLine(mPictureFrame, cvPoint(approx[k].x, approx[k].y), cvPoint(approx[k + 1].x, approx[k + 1].y), color);
                }
            }
        }
    emit framesReady();
    }

} // namespace media

