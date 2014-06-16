#ifndef ELLIPSEFRAME_H
#define ELLIPSEFRAME_H

#include "matrixframe.h"
#include <opencv/cv.h>

namespace media {

class EllipseFrame : public MatrixFrame
    {
public:
    enum { CenterX = 0, CenterY, SizeWidth, SizeHeight, AngleRotate, AngleFrom, AngleTo, VectorSize };

    explicit EllipseFrame(const FrameBase& aFrame);
    explicit EllipseFrame(const Matrix& aMatrix = Matrix()) { MatrixFrame::operator=(aMatrix); }

    double getStartAngleT(int aIndex) const
        {
        int point[Dimensions] = {aIndex, AngleFrom};
        return getSampleT(point);
        }

    double getEndAngle(int aIndex) const
        {
        int point[Dimensions] = {aIndex, AngleTo};
        return getSampleT(point);
        }

    cv::RotatedRect getRect(int aIndex) const;

    static QVector<double> toVect(const cv::RotatedRect& aRect, double aArcFrom, double aArcTo)
        {
        return QVector<double>() << aRect.center.x << aRect.center.y << aRect.size.width << aRect.size.height << aRect.angle << aArcFrom << aArcTo;
//        if (aArcFrom != aArcTo)
//            return QVector<double>() << 200 << 150 << 400 << 300 << 0 << 30 << 180;
//        else
//            return QVector<double>() << 400 << 300 << 300 << 400 << 0 << 30 << 180;
        }
    };

} // namespace media

#endif // ELLIPSEFRAME_H
