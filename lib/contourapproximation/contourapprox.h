#ifndef CONTOURBASE_H
#define CONTOURBASE_H

#include "contourpoints.h"

template <typename _Tp>
double meanSquare(const std::vector<cv::Point_<_Tp> >& v);

template <typename T>
std::vector<T> operator-(const std::vector<T>& v1, const std::vector<T>& v2);

template <typename _Tp, int ShapeMatchingMethod>
class ContourApprox : public ContourPoints<_Tp>
    {
    typedef cv::Point_<_Tp> PointType;

public:
    ContourApprox(const std::vector<PointType>& aPoints, double aTolerance = 0, cv::Size_<_Tp> aImgSize = cv::Size_<_Tp>()) :
        ContourPoints<_Tp>(aPoints),
        mTolerance(aTolerance),
        mImgSize(aImgSize)
        {
        }

    virtual ~ContourApprox() {}

    bool isApproximated() const { return mApprox.size() > 1; }
    const std::vector<PointType>& getApproxPoints() const { return mApprox; }
    virtual operator bool() const { return mApprox.size() > 0; }

    virtual int size() const { return mApprox.size(); }
    virtual const PointType &operator[](int aIndex) const { return mApprox.at(aIndex); }

    virtual void draw(IplImage *aIplImage, int aThickness, const CvScalar &aColor) const
        {
        for (int i = 0; i < ((int)mApprox.size() - 1); ++i)
            cvLine(aIplImage, cvPoint(mApprox[i].x, mApprox[i].y), cvPoint(mApprox[i + 1].x, mApprox[i + 1].y), aColor, aThickness);
        }

    bool operator< (const ContourApprox<_Tp, ShapeMatchingMethod>& aRight) const { return ShapeMatchingMethod ? getMatch() <  aRight.getMatch() : getFit() <  aRight.getFit(); }
    bool operator> (const ContourApprox<_Tp, ShapeMatchingMethod>& aRight) const { return ShapeMatchingMethod ? getMatch() >  aRight.getMatch() : getFit() >  aRight.getFit(); }
    bool operator<=(const ContourApprox<_Tp, ShapeMatchingMethod>& aRight) const { return ShapeMatchingMethod ? getMatch() <= aRight.getMatch() : getFit() <= aRight.getFit(); }
    bool operator>=(const ContourApprox<_Tp, ShapeMatchingMethod>& aRight) const { return ShapeMatchingMethod ? getMatch() >= aRight.getMatch() : getFit() >= aRight.getFit(); }
    bool operator==(const ContourApprox<_Tp, ShapeMatchingMethod>& aRight) const { return ShapeMatchingMethod ? getMatch() == aRight.getMatch() : getFit() == aRight.getFit(); }
    bool operator!=(const ContourApprox<_Tp, ShapeMatchingMethod>& aRight) const { return ShapeMatchingMethod ? getMatch() != aRight.getMatch() : getFit() != aRight.getFit(); }

    virtual double getFit() const { return meanSquare<_Tp>(this->mApprox - this->mPoints); }
    double getMatch() const { return cv::matchShapes(this->mApprox, this->mPoints, ShapeMatchingMethod, 0); }

protected:
    std::vector<PointType> mApprox;
    double mTolerance;
    cv::Size_<_Tp> mImgSize;
    };

template <typename _Tp>
double meanSquare(const std::vector<cv::Point_<_Tp> >& v)
    {
    double ret = 0;
    for (int i = 0; i < (int)v.size(); ++i)
        ret += norm(v[i])*norm(v[i]);
    return v.size() ? ret/v.size(): 0;
    }

template <typename T>
std::vector<T> operator-(const std::vector<T>& v1, const std::vector<T>& v2)
    {
    std::vector<T> ret;
//    if (v1.size() != v2.size()) return ret;
    for (int i = 0; i < std::min((int)v1.size(), (int)v2.size()); ++i)
        ret.push_back(v1[i] - v2[i]);
    return ret;
    }

#endif // CONTOURBASE_H
