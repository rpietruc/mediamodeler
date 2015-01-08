#ifndef IMGCONTOURPOINTS_H
#define IMGCONTOURPOINTS_H

#include <opencv2/imgproc/imgproc.hpp>

template <typename _Tp>
class ContourPoints
    {
    typedef cv::Point_<_Tp> PointType;

public:
    ContourPoints(const std::vector<PointType>& aPoints) :
        mPoints(aPoints)
        {
        unique(); // points must be unique
        }

    ContourPoints(const ContourPoints& aContour) :
        mPoints(aContour.mPoints)
        {
        unique(); // points must be unique
        }

    virtual ~ContourPoints() {}

    const std::vector<PointType>& getContourPoints() const { return mPoints; }
    cv::Size_<_Tp> getLineSize() const
        {
        return cv::Size_<_Tp>(mPoints.at(mPoints.size() - 1).x - mPoints.at(0).x, mPoints.at(mPoints.size() - 1).y - mPoints.at(0).y);
        }

    virtual int size() const { return mPoints.size(); }
    virtual const PointType &operator[](int aIndex) const { return mPoints.at(aIndex); }

    virtual void draw(IplImage *aIplImage, int aThickness, const CvScalar &aColor) const
        {
        for (int i = 0; i < ((int)mPoints.size() - 1); ++i)
            cvLine(aIplImage, cvPoint(mPoints[i].x, mPoints[i].y), cvPoint(mPoints[i + 1].x, mPoints[i + 1].y), aColor, aThickness);
        }

private:
    void unique()
        {
        //TODO: don't change an order
//        typename std::vector<PointType>::iterator last = std::unique(mPoints.begin(), mPoints.end());
//        mPoints.resize(std::distance(mPoints.begin(), last));
        }

protected:
    std::vector<PointType> mPoints;
    };

#endif // IMGCONTOURPOINTS_H
