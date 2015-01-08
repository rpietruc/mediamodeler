#ifndef ELLIPSECONTOUR_H
#define ELLIPSECONTOUR_H

#include "linecontour.h"
#include "closestpoint.h"
#include <gsl/gsl_sys.h>
#include "ellipsefitting.h"

inline int compareFloat(double val1, double val2, double aTolerance) { return gsl_fcmp(val1, val2, aTolerance); }
inline bool isSimilar(double val1, double val2, double aTolerance) { return (0 == compareFloat(val1, val2, aTolerance)); }

template <typename _Tp>
class Ellipse
    {
public:
    explicit Ellipse(const cv::RotatedRect& aRotatedRect = cv::RotatedRect()) :
        mRect(aRotatedRect) {}

    const Ellipse& operator=(const cv::RotatedRect& aRotatedRect)
        {
        mRect = aRotatedRect;
        return *this;
        }

    bool hasSimilarSize(const Ellipse &aEllipse, double aTolerance) const
        {
        return ::isSimilar((_Tp)cv::norm(cv::Point2f(getAxes().width, getAxes().height)), (_Tp)cv::norm(cv::Point2f(aEllipse.getAxes().width, aEllipse.getAxes().height)), aTolerance);
        }

    bool hasSimilarCenter(const Ellipse &aEllipse, double aTolerance, cv::Size_<_Tp> aImgSize) const
        {
        return (_Tp)cv::norm(getCenter() - aEllipse.getCenter())/(_Tp)cv::norm(cv::Point2f(aImgSize.width, aImgSize.height)) < aTolerance;
        }

    bool hasSimilarAngle(const Ellipse &aEllipse, double aTolerance) const
        {
        return ::isSimilar(getAngle(), aEllipse.getAngle(), aTolerance);
        }

    bool isSimilar(const Ellipse &aEllipse, double aTolerance = 0, cv::Size_<_Tp> aImgSize = cv::Size_<_Tp>()) const
        {
        if (aTolerance > 0)
            return hasSimilarSize(aEllipse, aTolerance) &&
                   hasSimilarCenter(aEllipse, aTolerance, aImgSize) &&
                   hasSimilarAngle(aEllipse, aTolerance);
        //else
        return operator ==(aEllipse);
        }

    bool operator==(const Ellipse &aEllipse) const
        {
            return
                ((_Tp)getAxes().height == (_Tp)aEllipse.getAxes().height) &&
                ((_Tp)getAxes().width == (_Tp)aEllipse.getAxes().width) &&
                ((_Tp)getCenter().x == (_Tp)aEllipse.getCenter().x) &&
                ((_Tp)getCenter().y == (_Tp)aEllipse.getCenter().y) &&
                ((_Tp)getAngle() == (_Tp)aEllipse.getAngle());
        }

    LineNormalForm<_Tp> majorAxis() const
        {
        LineNormalForm<_Tp> ret(cv::Point_<_Tp>(getCenter().x, getCenter().y), cv::Point_<_Tp>(getCenter().x + getAxes().width, getCenter().y));
        return ret.rotate(getAngle());
        }

    operator cv::RotatedRect() const { return mRect; }

    cv::Point_<_Tp> getCenter() const { return cv::Point_<_Tp>(mRect.center.x, mRect.center.y); }
    cv::Size_<_Tp> getAxes() const { return cv::Size_<_Tp>(mRect.size.width/2, mRect.size.height/2); }
    _Tp getAngle() const { return mRect.angle; }

private:
    cv::RotatedRect mRect;
    };

template <typename _Tp>
class Arcs
    {
public:
    Arcs() {}
    Arcs(const std::vector<_Tp>& aAngles) { this->operator =(aAngles); }
    Arcs(const Arcs &aArcs) { this->operator =(aArcs.mAngles); }

    const Arcs& operator=(const std::vector<_Tp>& aAngles)
        {
        mInverted = false;
        mAngles = aAngles;
        if (mAngles.size() > 1)
            for (typename std::vector<_Tp>::iterator iter = mAngles.begin(); iter != (mAngles.end() - 1); ++iter)
                {
                _Tp prev = *iter;
                _Tp next = *(iter + 1);
                if (((next - prev) > 180) || ((next - prev) < -180))
                    mInverted = true;
                mDist.push_back(arcDist(prev, next));
                }
        return *this;
        }

    bool operator==(const Arcs &aArcs) const
        {
        assert(mAngles.size() > 1);
        assert(aArcs.mAngles.size() > 1);
        return
            ((*mAngles.begin() == *aArcs.mAngles.begin()) && (*(mAngles.end() - 1) == *(aArcs.mAngles.end() - 1))) ||
            ((*mAngles.begin() == *(aArcs.mAngles.end() - 1)) && (*(mAngles.end() - 1) == *aArcs.mAngles.begin()));
        }

    bool similar(const Arcs &aArcs, double aTolerance) const
        {
        return
            ((::isSimilar(*mAngles.begin(), *aArcs.mAngles.begin(), aTolerance) && ::isSimilar(*(mAngles.end() - 1), *(aArcs.mAngles.end() - 1), aTolerance)) ||
             (::isSimilar(*mAngles.begin(), *(aArcs.mAngles.end() - 1), aTolerance) && ::isSimilar(*(mAngles.end() - 1), *aArcs.mAngles.begin(), aTolerance)));
        }

    bool sorted() const
        {
        if (mDist.size() <= 2)
            return true;
        for (int i = 0; i < ((int)mDist.size() - 1); ++i)
            if ((mDist.at(i) * mDist.at(i + 1)) < -180)
                return false;
        return true;
        }

    _Tp arcFrom() const { return mInverted ? bound(*mAngles.begin()) : *mAngles.begin(); }
    _Tp arcTo() const { return mInverted ? bound(*(mAngles.end() - 1)) : *(mAngles.end() - 1); }

    static _Tp positive(_Tp aAngle)
        {
        while (aAngle < 0) aAngle += 360;
        while (aAngle > 360) aAngle -= 360;
        return aAngle;
        }

    static _Tp bound(_Tp aAngle)
        {
        while (aAngle < -180) aAngle += 360;
        while (aAngle > 180) aAngle -= 360;
        return aAngle;
        }

    static _Tp arcDist(_Tp aFrom, _Tp aTo) { return bound(positive(aTo) - positive(aFrom)); }

private:
    std::vector<_Tp> mAngles;
    std::vector<_Tp> mDist;
    bool mInverted;
    };

template <typename _Tp, int ShapeMatchingMethod>
class EllipseContour : public ContourApprox<_Tp, ShapeMatchingMethod>
    {
    typedef cv::Point_<_Tp> PointType;

public:
    explicit EllipseContour(const std::vector<PointType>& aPoints = std::vector<PointType>(), int aAlgorithm = 0, double aTolerance = 0, cv::Size_<_Tp> aImgSize = cv::Size_<_Tp>()) :
        ContourApprox<_Tp, ShapeMatchingMethod>(aPoints, aTolerance, aImgSize),
        mAlgorithm(aAlgorithm)
        {
        if (aPoints.size() > 5)
            {
            // Developer should keep in mind that it is possible
            // that the returned ellipse/rotatedRect data contains negative indices,
            // due to the data points being close to the border of the containing Mat element.
            cv::RotatedRect rect =  mAlgorithm ? ::fitEllipse(aPoints) : cv::fitEllipse(aPoints);
            mEllipse = rect;

            // sometimes it fits ellipse with 0 area (line segment)
            if ((mEllipse.getAxes().width > 0) && (mEllipse.getAxes().height > 0))
                {
                std::vector<_Tp> arcs;
                for (size_t i = 0; i < aPoints.size(); ++i)
                    {
                    ofVec2f ellipsePoint = closestPointOnEllipse(rect, ofVec2f(aPoints[i].x, aPoints[i].y));
                    PointType point(ellipsePoint.x, ellipsePoint.y);
                    this->mApprox.push_back(point);
                    arcs.push_back(arcAngleFromMajorAxisToPoint(point));
                    }
                mArcs = Arcs<_Tp>(arcs);
                }
            }
        }
    operator Ellipse<_Tp>() const { return mEllipse; }

    bool operator< (const LineContour<_Tp, ShapeMatchingMethod>& aRight) const { return *this && mArcs.sorted() ? ContourApprox<_Tp, ShapeMatchingMethod>::operator< (aRight) : false; }
    bool operator> (const LineContour<_Tp, ShapeMatchingMethod>& aRight) const { return *this && mArcs.sorted() ? ContourApprox<_Tp, ShapeMatchingMethod>::operator> (aRight) : true;  }
    bool operator<=(const LineContour<_Tp, ShapeMatchingMethod>& aRight) const { return *this && mArcs.sorted() ? ContourApprox<_Tp, ShapeMatchingMethod>::operator<=(aRight) : false; }
    bool operator>=(const LineContour<_Tp, ShapeMatchingMethod>& aRight) const { return *this && mArcs.sorted() ? ContourApprox<_Tp, ShapeMatchingMethod>::operator>=(aRight) : true;  }
    bool operator==(const LineContour<_Tp, ShapeMatchingMethod>& aRight) const { return *this && mArcs.sorted() ? ContourApprox<_Tp, ShapeMatchingMethod>::operator==(aRight) : false; }
    bool operator!=(const LineContour<_Tp, ShapeMatchingMethod>& aRight) const { return *this && mArcs.sorted() ? ContourApprox<_Tp, ShapeMatchingMethod>::operator!=(aRight) : true;  }

    bool operator==(const EllipseContour &aEllipse) const
        {
        if (*this && aEllipse && mArcs.sorted() && aEllipse.getThetas().sorted())
            return mEllipse.isSimilar(aEllipse, this->mTolerance, this->mImgSize)
              /*&& mArcs.similar(aEllipse.mArcs, this->mTolerance)*/;
        return false;
        }

    EllipseContour operator+(const EllipseContour &aEllipse) const
        {
        std::vector<PointType> points(ContourPoints<_Tp>::mPoints);
        points.insert(points.end(), aEllipse.mPoints.begin(), aEllipse.mPoints.end());
        return EllipseContour<_Tp, ShapeMatchingMethod>(points, ContourApprox<_Tp, ShapeMatchingMethod>::mTolerance, mAlgorithm, ContourApprox<_Tp, ShapeMatchingMethod>::mImgSize);
        }

    void draw(IplImage *aIplImage, int aThickness, const CvScalar &aColor) const
        {
        CvSize axes = {(int)operator Ellipse<_Tp>().getAxes().width, (int)operator Ellipse<_Tp>().getAxes().height};
        cvEllipse(aIplImage, operator Ellipse<_Tp>().getCenter(), axes, operator Ellipse<_Tp>().getAngle(), mArcs.arcFrom(), mArcs.arcTo(), aColor, aThickness);
        }

    const Arcs<_Tp>& getThetas() const { return mArcs; }

private:
    _Tp arcAngleFromMajorAxisToPoint(const cv::Point_<_Tp> &aPointOnEllipse) const
        {
        LineNormalForm<_Tp> line(cv::Point_<_Tp>(mEllipse.getCenter().x, mEllipse.getCenter().y), aPointOnEllipse);
        return line.angleFromLine(mEllipse.majorAxis());
        }

private:
    Ellipse<_Tp> mEllipse;
    Arcs<_Tp> mArcs;
    int mAlgorithm;
    };

#endif // ELLIPSECONTOUR_H
