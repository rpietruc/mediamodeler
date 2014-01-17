#ifndef CONTOURBASE_H
#define CONTOURBASE_H

#include "contourpoints.h"

class ContourApprox : public ContourPoints
    {
public:
    ContourApprox(const std::vector<cv::Point>& aPoints) : ContourPoints(aPoints) {}
    ContourApprox(const ContourPoints& aContour) : ContourPoints(aContour) {}
    ContourApprox(const ContourApprox& aContour) : ContourPoints(aContour) {}

    virtual ~ContourApprox() {}

    bool isApproximated() const { return mApprox.size() > 1; }
    const std::vector<cv::Point>& getApproxPoints() const { return mApprox; }

protected:
    std::vector<cv::Point> mApprox;
    };

#endif // CONTOURBASE_H
