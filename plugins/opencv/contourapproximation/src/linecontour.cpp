#include "linecontour.h"
#include "ofVec2f.h"

using namespace std;
using namespace cv;

ofVec2f closestPointOnLine(const ofVec2f& p1, const ofVec2f& p2, const ofVec2f& p3) {
    if(p1 == p2) {
        return p1;
    }

    float u = (p3.x - p1.x) * (p2.x - p1.x);
    u += (p3.y - p1.y) * (p2.y - p1.y);
    float len = (p2 - p1).length();
    u /= (len * len);

    // clamp u
    if(u > 1) {
        u = 1;
    } else if(u < 0) {
        u = 0;
    }
    return p1.getInterpolated(p2, u);
}

LineContour::LineContour(const vector<Point>& aPoints) :
    ContourApprox(aPoints)
    {
    if (mPoints.size() > 1)
        {
        mLine.push_back(mPoints[0]);
        mLine.push_back(mPoints[mPoints.size() - 1]);
        for (size_t i = 0; i < aPoints.size(); ++i)
            {
            ofVec2f linePoint = closestPointOnLine(ofVec2f(mLine[0].x, mLine[0].y), ofVec2f(mLine[1].x, mLine[1].y), ofVec2f(aPoints[i].x, aPoints[i].y));
            //double dist = norm(points[i] - ellipsePoint);
            mApprox.push_back(Point(linePoint.x, linePoint.y));
            }
        }
    }
