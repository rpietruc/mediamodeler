#ifndef IMGCONTOURPOINTS_H
#define IMGCONTOURPOINTS_H

#include <opencv2/imgproc/imgproc.hpp>

class ContourPoints
    {
public:
    ContourPoints(const std::vector<cv::Point>& aPoints) : mPoints(aPoints) {}
    ContourPoints(const ContourPoints& aContour) : mPoints(aContour.mPoints) {}

    virtual ~ContourPoints() {}

    const std::vector<cv::Point>& getContourPoints() const { return mPoints; }

protected:
    std::vector<cv::Point> mPoints;
    };

#endif // IMGCONTOURPOINTS_H
