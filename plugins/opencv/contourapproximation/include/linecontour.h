#ifndef LINECONTOUR_H
#define LINECONTOUR_H

#include "contourapprox.h"

class LineContour : public ContourApprox
    {
public:
    LineContour(const std::vector<cv::Point>& aPoints);
    virtual ~LineContour() {}

protected:
    std::vector<cv::Point> mLine;
    };

#endif // LINECONTOUR_H

