#include "closestpoint.h"

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

ofVec2f closestPointOnEllipse(const cv::RotatedRect& ellipse, const ofVec2f& point) {
    ofVec2f norm = point;
    ofVec2f offset(ellipse.center.x, ellipse.center.y);
    norm -= offset;
    norm.rotate(-ellipse.angle);
    bool flipX = norm.x < 0, flipY = norm.y < 0;
    if(flipX) norm.x *= -1;
    if(flipY) norm.y *= -1;
    float e[] = {ellipse.size.width / 2, ellipse.size.height / 2};
    float y[] = {norm.x, norm.y};
    float x[2];
    DistancePointEllipse(e, y, x);
    ofVec2f result(x[0], x[1]);
    if(flipX) result.x *= -1;
    if(flipY) result.y *= -1;
    result.rotate(ellipse.angle);
    result += offset;
    return result;
    }
