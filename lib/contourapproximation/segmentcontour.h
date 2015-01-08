#ifndef SEGMENTCONTOUR_H
#define SEGMENTCONTOUR_H

#include "contourapprox.h"

template <typename _Tp, int ShapeMatchingMethod = 0>
class SegmentContour : public ContourApprox<_Tp, ShapeMatchingMethod>
    {
    typedef cv::Point_<_Tp> PointType;

public:
    SegmentContour(const std::vector<PointType>& aPoints, double aAccuracy, bool aClosed) :
        ContourApprox<_Tp, ShapeMatchingMethod>(aPoints)
        {
        // approximate contour with accuracy proportional to the contour perimeter
        cv::approxPolyDP(cv::Mat(this->mPoints), this->mApprox, aAccuracy, aClosed);
        typename std::vector<PointType>::iterator first = this->mPoints.begin();
        typename std::vector<PointType>::iterator last = this->mPoints.begin();
        for (size_t i = 0; (i < (this->mApprox.size() - 1)) && (first != this->mPoints.end()) && (last != this->mPoints.end()); ++i)
            {
            first = std::find(first, this->mPoints.end(), this->mApprox[i]);
            last = std::find(first, this->mPoints.end(), this->mApprox[i + 1]);
            if ((first != this->mPoints.end()) && (last != this->mPoints.end()) && (last > first))
                mSegments.push_back(std::vector<PointType>(first, last + 1));
            }
        }

    const std::vector<std::vector<PointType> >& getSegments() const { return mSegments; }

protected:
    std::vector<std::vector<PointType> > mSegments;
    };

#endif // SEGMENTCONTOUR_H
