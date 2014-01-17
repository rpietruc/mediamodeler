#ifndef SEGMENTCONTOUR_H
#define SEGMENTCONTOUR_H

#include "contourapprox.h"

class SegmentContour : public ContourApprox
    {
public:
    SegmentContour(const std::vector<cv::Point>& aPoints, double aAccuracy);
    SegmentContour(const ContourPoints& aContour, double aAccuracy);

    const std::vector<std::vector<cv::Point> >& getSegments() const { return mSegments; }

private:
    void init();

protected:
    std::vector<std::vector<cv::Point> > mSegments;

private:
    double mAccuracy;
    };

#endif // SEGMENTCONTOUR_H
