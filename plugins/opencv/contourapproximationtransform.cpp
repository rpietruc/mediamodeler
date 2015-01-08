#include "contourapproximationtransform.h"
#include "segmentcontour.h"
#include "ellipsecontour.h"
#include <pictureframes.h>
#include <QDebug>
#include <opencv/cv.h>

using namespace cv;
using namespace std;

namespace media {

typedef int ValueType;
typedef cv::Point_<ValueType> PointType;
typedef cv::Size_<ValueType> SizeType;
static const int ShapeMatchingMethod = 0;
typedef ContourApprox<ValueType, ShapeMatchingMethod> ContourApproxType;
typedef LineContour<ValueType, ShapeMatchingMethod> LineContourType;
typedef EllipseContour<ValueType, ShapeMatchingMethod> EllipseContourType;

template <typename _Tp>
vector<vector<Point_<_Tp> > > splitContour(const vector<Point_<_Tp> > &aContour, double aAccuracy, bool aClosed)
    {
    vector<vector<Point_<_Tp> > >contours;
    SegmentContour<_Tp> segment(aContour, aAccuracy, aClosed);
    for (size_t j = 0; j < segment.getSegments().size(); ++j)
        contours.push_back(segment.getSegments()[j]);
    return contours;
    }

template <typename _Tp>
void removeDuplicates(vector<_Tp> &aValues)
    {
    bool cont;
    do
        {
        cont = false;
        for (int i = 0; i < ((int)aValues.size() - 1); ++i)
            {
            for (int j = i + 1; j < ((int)aValues.size() - 1); ++j)
                if (aValues.at(i) == aValues.at(j))
                    {
                    aValues.erase(aValues.begin() + j);
                    cont = true;
                    break;
                    }
            if (cont)
                break;
            }
        }
    while (cont);
    }

template <typename _Tp>
void mergeCongruent(vector<_Tp> &aValues)
    {
    bool cont;
    do
        {
        cont = false;
        for (int i = 0; i < ((int)aValues.size() - 1); ++i)
            if (aValues.at(i) == aValues.at(i + 1))
                {
                aValues[i] = aValues.at(i) + aValues.at(i + 1);
                aValues.erase(aValues.begin() + i + 1);
                cont = true;
                }
        }
    while (cont);
    }

ContourApproximationTransform::ContourApproximationTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("accuracy", 0.04);
    setProperty("tolerance", 0.25);
    setProperty("algorithm", 0);
    }

void ContourApproximationTransform::process()
    {
    vector<vector<PointType> > contours;
    SizeType imgSize(0, 0);
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (PictureRGBFrame().isCopyable(*frame))
                  imgSize = SizeType(frame->getDimensionT(PictureRGBFrame::Width).mResolution,
                                     frame->getDimensionT(PictureRGBFrame::Height).mResolution);

            if ((frame->getMaxDimension() == PointsFrame::Dimensions) &&
                (frame->getDimensionT(PointsFrame::Axis).mResolution == PointsFrame::MaxAxis))
                {
                vector<PointType> contour;
                contour.resize(frame->getDimensionT(PointsFrame::Index).mResolution);
                int point[PointsFrame::Dimensions] = {0, 0};
                for (point[PointsFrame::Index] = 0; point[PointsFrame::Index] < frame->getDimensionT(PointsFrame::Index).mResolution; ++point[PointsFrame::Index])
                    {
                    point[PointsFrame::Axis] = PointsFrame::XAxis;
                    contour[point[PointsFrame::Index]].x = frame->getSampleT(point);
                    point[PointsFrame::Axis] = PointsFrame::YAxis;
                    contour[point[PointsFrame::Index]].y = frame->getSampleT(point);
                    }
                if (contour.size() > 1)
                    contours.push_back(contour);
                }
            }

    mEllipseFrames.clear();
    mLineSegmentFrames.clear();
    int ellipsesNo = 0;
    int linesNo = 0;

//    emit logMessage(Qt::red, QString("contours no. %1").arg(contours.size()));
    if (contours.size() > 0)
        {
        for (int i = 0; i < (int)contours.size(); ++i)
            {
            Matrix ellipseMatrix;
            Matrix lineSegmentMatrix;
            //
            // TODO: find if the contour is closed
            //
            bool closedContour = false;
            double accuracy = property("accuracy").toDouble();
//            accuracy *= arcLength(Mat(contours[i]), closedContour);
//            emit logMessage(Qt::red, QString("accuracy %1").arg(accuracy));

            vector<vector<PointType> > segments = splitContour(contours[i], accuracy, closedContour);
            vector<EllipseContourType> approx;
            for (int i = 0; i < (int)segments.size(); ++i)
                {
                approx.push_back(EllipseContourType(segments.at(i), property("algorithm").toInt(), property("tolerance").toDouble(), imgSize));
//                emit logMessage(Qt::red, QString("ellipse from %1 to %2").arg(approx.at(approx.size() - 1).getThetas().arcFrom()).arg(approx.at(approx.size() - 1).getThetas().arcTo()));
                }

            if (property("tolerance").toDouble() > 0)
                {
//                int no = approx.size();
                mergeCongruent(approx);
//                emit logMessage(Qt::red, QString("compression rate %1/%2").arg(approx.size()).arg(no));
                }

            for (int i = 0; i < (int)approx.size(); ++i)
                {
                LineContourType lineSegment(approx[i].getContourPoints());
                if (approx[i] <= lineSegment)
                    {
                    ellipseMatrix.push_back(EllipseFrame::toVect((Ellipse<ValueType>)approx[i], approx[i].getThetas().arcFrom(), approx[i].getThetas().arcTo()));
//                    emit logMessage(Qt::red, QString("ellipse theta %1 %2").arg(approx[i].getThetas().arcFrom()).arg(approx[i].getThetas().arcTo()));
                    ++ellipsesNo;
                    }
                else if (lineSegment.getContourPoints().size() >= 2)
                    {
                    QPointF startPoint(lineSegment.getContourPoints().at(0).x,
                                       lineSegment.getContourPoints().at(0).y);

                    QPointF endPoint(lineSegment.getContourPoints().at(lineSegment.getContourPoints().size() - 1).x,
                                     lineSegment.getContourPoints().at(lineSegment.getContourPoints().size() - 1).y);

                    lineSegmentMatrix.push_back(LineSegmentFrame::toVect(startPoint, endPoint));
                    ++linesNo;
                    }
                }
            if (ellipseMatrix.size() > 0)
                mEllipseFrames.push_back(EllipseFrame(ellipseMatrix));

            if (lineSegmentMatrix.size() > 0)
                mLineSegmentFrames.push_back(LineSegmentFrame(lineSegmentMatrix));
            }
        emit logMessage(Qt::red, QString("total: %1 ellipses and %2 line segments").arg(ellipsesNo).arg(linesNo));
        emit framesReady();
        }
    }

} // namespace media
