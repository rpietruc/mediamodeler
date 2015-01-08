#ifndef LINECONTOUR_H
#define LINECONTOUR_H

#include "contourapprox.h"
#include "closestpoint.h"

template <typename _Tp>
class LineNormalForm
    {
public:
    LineNormalForm() {}
    LineNormalForm(const LineNormalForm& aCopy) :
        mDistanceFromOrigin(aCopy.mDistanceFromOrigin),
        mAngle(aCopy.mAngle) {}
    LineNormalForm(const cv::Point_<_Tp> &aPoint1, const cv::Point_<_Tp> &aPoint2) { operator=(std::make_pair(aPoint1, aPoint2)); }

    _Tp distanceFromOrigin() const { return mDistanceFromOrigin; }
    _Tp angle() const { return mAngle; }

    const LineNormalForm& operator=(const std::pair<cv::Point_<_Tp>, cv::Point_<_Tp> > &aPoints)
        {
        mAngle = angleFromPoints(aPoints.first, aPoints.second);
        mDistanceFromOrigin = distanceFromPointAndAngle(cv::Point_<_Tp>(aPoints.first.x, aPoints.first.y), mAngle);
        return *this;
        }

    bool isParallel(const LineNormalForm& aLine) const { return aLine.angle() == mAngle; }
    bool operator==(const LineNormalForm &aLine) const { return isParallel(aLine) && (aLine.distanceFromOrigin() == mDistanceFromOrigin); }

    _Tp  angleFromLine(const LineNormalForm &aLine) const
        {
        return mAngle - aLine.mAngle;
        }

    const LineNormalForm& rotate(_Tp aAngle)
        {
        mAngle = mAngle + aAngle;
        return *this;
        }

private:
    _Tp angleFromPoints(const cv::Point_<_Tp> &aPoint1, const cv::Point_<_Tp> &aPoint2)
        {
        return RAD_TO_DEG*atan2(aPoint2.x - aPoint1.x, aPoint1.y - aPoint2.y);
        }

    _Tp distanceFromPointAndAngle(const cv::Point_<_Tp>& aPoint, _Tp  aAngle)
        {
        return aPoint.y*sin(DEG_TO_RAD*aAngle) + aPoint.x*cos(DEG_TO_RAD*aAngle);
        }

private:
    _Tp mDistanceFromOrigin;
    _Tp mAngle;
    };

//template <typename _Tp>
//class LineSegment
//    {
//    typedef cv::Point_<_Tp> PointType;

//public:
//    explicit LineSegment(const PointType& p1, const PointType& p2) :
//        mPoints(std::make_pair(p1, p2))
//        {}

//    explicit LineSegment(const LineSegment& aLineSegment) :
//        mPoints(aLineSegment.mPoints)
//        {}

//    const std::pair<PointType, PointType> &getPoints() const { return mPoints; }

//protected:
//    std::pair<PointType, PointType> mPoints;
//    };

template <typename _Tp, int ShapeMatchingMethod>
class LineContour : public ContourApprox<_Tp, ShapeMatchingMethod>
    {
    typedef cv::Point_<_Tp> PointType;

public:
    explicit LineContour(const std::vector<PointType>& aPoints = std::vector<PointType>()) :
        ContourApprox<_Tp, ShapeMatchingMethod>(aPoints)
        {
        if (aPoints.size() > 1)
            {
            for (size_t i = 0; i < aPoints.size(); ++i)
                {
                ofVec2f linePoint = closestPointOnLine(ofVec2f(aPoints[0].x, aPoints[0].y), ofVec2f(aPoints[aPoints.size() - 1].x, aPoints[aPoints.size() - 1].y), ofVec2f(aPoints[i].x, aPoints[i].y));
                this->mApprox.push_back(PointType(linePoint.x, linePoint.y));
                }
            }
        }

    operator LineNormalForm<_Tp>() const { return LineNormalForm<_Tp>(this->mPoints[0], this->mPoints[this->mPoints.size() - 1]); }
//    operator LineSegment<_Tp>() const { return LineSegment<_Tp>(this->mPoints[0], this->mPoints[this->mPoints.size() - 1]); }

    virtual void draw(IplImage *aIplImage, int aThickness, const CvScalar &aColor) const
        {
        if (this->mPoints.size() > 1)
            cvLine(aIplImage, cvPoint(this->mPoints[0].x, this->mPoints[0].y), cvPoint(this->mPoints[this->mPoints.size() - 1].x, this->mPoints[this->mPoints.size() - 1].y), aColor, aThickness);
        }
    };

#endif // LINECONTOUR_H
