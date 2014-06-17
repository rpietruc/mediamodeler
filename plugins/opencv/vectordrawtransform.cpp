#include "vectordrawtransform.h"
#include <ellipseframe.h>
#include <linesegmentframe.h>

using namespace cv;
using namespace std;

namespace media {

typedef int ValueType;
typedef cv::Point_<ValueType> PointType;
typedef cv::Size_<ValueType> SizeType;

CvScalar randomColor()
    {
    CvScalar color = {static_cast<double>(qrand()%256), static_cast<double>(qrand()%256), static_cast<double>(qrand()%256), static_cast<double>(qrand()%256)};
    return color;
    }

VectorDrawTransform::VectorDrawTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    }

void VectorDrawTransform::process()
    {
    QVector<EllipseFrame> ellipses;
    QVector<LineSegmentFrame> lineSegments;

    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);

            if ((frame->getMaxDimension() == EllipseFrame::Dimensions) &&
                (frame->getDimensionT(EllipseFrame::ParamNo).mResolution == EllipseFrame::VectorSize))
                ellipses.push_back(EllipseFrame(*frame));

            else if ((frame->getMaxDimension() == LineSegmentFrame::Dimensions) &&
                (frame->getDimensionT(LineSegmentFrame::ParamNo).mResolution == LineSegmentFrame::VectorSize))
                lineSegments.push_back(LineSegmentFrame(*frame));

            else if (frame->getMaxDimension() == IplImageFrame::Dimensions)
                {
                mPictureFrame.resizeAndCopyFrame(*frame);
                mPictureFrame.clear();
                mPictureFrame.setSourceName(frame->getSourceName());
                }
            }

    int ellipsesNo = 0;
    foreach (EllipseFrame frame, ellipses)
        for (int i = 0; i < frame.getDimensionT(EllipseFrame::VectorNo).mResolution; ++i, ++ellipsesNo)
            cvEllipse(mPictureFrame, frame.getRect(i).center, cvSize(frame.getRect(i).size.width/2, frame.getRect(i).size.height/2), frame.getRect(i).angle, frame.getStartAngleT(i), frame.getEndAngle(i), randomColor(), 2);

    int linesNo = 0;
    foreach (LineSegmentFrame frame, lineSegments)
        for (int i = 0; i < frame.getDimensionT(LineSegmentFrame::VectorNo).mResolution; ++i, ++linesNo)
            cvLine(mPictureFrame, cvPoint(frame.getStartPointT(i).x(), frame.getStartPointT(i).y()), cvPoint(frame.getEndPointT(i).x(), frame.getEndPointT(i).y()), randomColor(), 1);

    emit logMessage(Qt::red, QString("total: %1 ellipses and %2 line segments").arg(ellipsesNo).arg(linesNo));
    emit framesReady();
    }

} // namespace media
