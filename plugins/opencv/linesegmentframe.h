#ifndef LINESEGMENTFRAME_H
#define LINESEGMENTFRAME_H

#include "matrixframe.h"
#include <QPoint>

namespace media {

class LineSegmentFrame : public MatrixFrame
    {
public:
    enum { StartPointX = 0, StartPointY, EndPointX, EndPointY, VectorSize };

    explicit LineSegmentFrame() {}
    explicit LineSegmentFrame(const FrameBase& aFrame);
    explicit LineSegmentFrame(const Matrix& aMatrix) { MatrixFrame::operator=(aMatrix); }

    QPointF getStartPointT(int aIndex) const
        {
        int pointX[Dimensions] = {aIndex, StartPointX};
        int pointY[Dimensions] = {aIndex, StartPointY};
        return QPointF(getSampleT(pointX), getSampleT(pointY));
        }

    QPointF getEndPointT(int aIndex) const
        {
        int pointX[Dimensions] = {aIndex, EndPointX};
        int pointY[Dimensions] = {aIndex, EndPointY};
        return QPointF(getSampleT(pointX), getSampleT(pointY));
        }

    static QVector<double> toVect(const QPointF& aStartPoint, const QPointF& aEndPoint)
        {
        return QVector<double>() << aStartPoint.x() << aStartPoint.y() << aEndPoint.x() << aEndPoint.y();
        }

    bool isCopyable(const FrameBase& aFrame) const
        {
        return FrameBase::isCopyable(aFrame) &&
            (aFrame.getDimensionT(ParamNo).mResolution == VectorSize);
        }
    };

} // namespace media

#endif // LINESEGMENTFRAME_H
