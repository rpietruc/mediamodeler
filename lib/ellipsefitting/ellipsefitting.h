#ifndef ELLIPSEFITTING_H
#define ELLIPSEFITTING_H

#include <armadillo>
#include <vector>
#include <opencv2/core/core.hpp>

void fitEllipse(const arma::mat &aPoints, arma::colvec2 &z, float &a, float &b, float &alpha);

template <typename _Tp>
cv::RotatedRect fitEllipse(const std::vector<cv::Point_<_Tp> > &aPoints)
    {
    cv::RotatedRect ret;

    arma::mat x(aPoints.size(), 2);
    for (int i = 0; i < (int)aPoints.size(); ++i)
        {
        arma::rowvec2 row;
        row << aPoints.at(i).x << aPoints.at(i).y;
        x.row(i) = row;
        }
    arma::colvec2 z;
    float a, b, alpha;
    fitEllipse(x, z, a, b, alpha);

    ret.center = cv::Point2f(z(0), z(1));
    ret.size = cv::Size2f(2*a, 2*b);
    ret.angle = (180.0/arma::datum::pi)*alpha;
    return ret;
    }

#endif // ELLIPSEFITTING_H
