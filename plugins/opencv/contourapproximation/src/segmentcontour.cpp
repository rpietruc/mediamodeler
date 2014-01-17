#include "segmentcontour.h"
//#include <iostream>

using namespace std;
using namespace cv;

SegmentContour::SegmentContour(const std::vector<cv::Point>& aPoints, double aAccuracy) :
    ContourApprox(aPoints),
    mAccuracy(aAccuracy)
    {
    init();
    }

SegmentContour::SegmentContour(const ContourPoints& aContour, double aAccuracy) :
    ContourApprox(aContour),
    mAccuracy(aAccuracy)
    {
    init();
    }

void SegmentContour::init()
    {
    // approximate contour with accuracy proportional
    // to the contour perimeter
//    approxPolyDP(Mat(mPoints), mApprox, arcLength(Mat(mPoints), true)*mAccuracy, false);
    approxPolyDP(Mat(mPoints), mApprox, mAccuracy, false);
    size_t prev = 0;
    for (size_t i = 0; i < (getApproxPoints().size() - 1); ++i)
        {
        vector<Point> segment;
        Point approxFirst = getApproxPoints()[i];
        Point approxLast = getApproxPoints()[i + 1];

        //find first point
        while ((prev < (mPoints.size() - 1) && (mPoints[prev] != approxFirst)))
            ++prev;
        if (prev >= (mPoints.size() - 1))
            break;
        segment.push_back(mPoints[prev]);

        //append next points
        size_t next = prev + 1;
        while ((next < mPoints.size()) && (mPoints[next] != approxLast))
            segment.push_back(mPoints[next++]);
        if (next >= mPoints.size())
            break;
        segment.push_back(mPoints[next++]);

        mSegments.push_back(segment);
        }
    }
