#ifndef ELLIPSECONTOUR_H
#define ELLIPSECONTOUR_H

#include "contourapprox.h"

class EllipseContour : public ContourApprox
    {
public:
    EllipseContour(const std::vector<cv::Point>& aPoints);
    virtual ~EllipseContour() {}

protected:
    cv::RotatedRect mRect;
    };

#endif // ELLIPSECONTOUR_H
