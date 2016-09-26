#include "multiplechannelpcnn.h"
#include <numeric>

using namespace cv;
using namespace std;
using namespace mpcnn;

namespace mpcnn
{

Mat getSeededPixels(const Mat& rgbImg, double theta_mu)
    {
    vector<Mat> neighbours;
    getAllUnitShifts(rgbImg, neighbours);
    vector<Mat> absdiffs;
    getAbsDiffs(rgbImg, neighbours, absdiffs);
    return (getMaxChannelsSum(absdiffs) < theta_mu);
    }

Mat sumChannels(const Mat& mat)
    {
    Mat mout;
    reduce(mat.reshape(1, mat.total()), mout, 1, CV_REDUCE_SUM, CV_32F);
    return mout.reshape(1, mat.rows);
    }

void getAllUnitShifts(const Mat& rgbImg, vector<Mat> &shifts)
    {
    vector<int> indexes(9);
    iota(indexes.begin(), indexes.end(), 0); // (0..8)
    indexes.erase(indexes.begin() + 4); // (0..3, 5..8)

    vector<CvPoint> points;
    transform(indexes.begin(), indexes.end(), back_inserter(points), [](int i) { return cvPoint(i/3, i%3); });
    // (0, 0) -> [1,0,0; 0,0,0; 0,0,0]
    // (0, 1) -> [0,0,0; 1,0,0; 0,0,0]
    // (0, 2) -> [0,0,0; 0,0,0; 1,0,0]
    // (1, 0) -> [0,1,0; 0,0,0; 0,0,0]
    // (1, 1) -> [0,0,0; 0,1,0; 0,0,0] <-- erased (identical shift)
    // (1, 2) -> [0,0,0; 0,0,0; 0,1,0]
    // (2, 0) -> [0,0,1; 0,0,0; 0,0,0]
    // (2, 1) -> [0,0,0; 0,0,1; 0,0,0]
    // (2, 2) -> [0,0,0; 0,0,0; 0,0,1]

    transform(points.begin(), points.end(), back_inserter(shifts),
        [rgbImg] (const CvPoint &delta)
            {
            Mat kernel(3, 3, rgbImg.depth(), Scalar().all(0));
            kernel.at<float>(1, 1) = 1.0f;
            Mat shift = Mat(rgbImg.rows, rgbImg.cols, rgbImg.type(), Scalar().all(0));
            filter2D(rgbImg, shift, -1 , kernel, delta, 0, BORDER_CONSTANT);
            return shift;
            });
    }

void getAbsDiffs(const Mat& rgbImg, const vector<Mat> &neighbours, vector<Mat> &absdiffs)
    {
    transform(neighbours.begin(), neighbours.end(), back_inserter(absdiffs),
        [rgbImg](const Mat& elem)
            {
            Mat res;
            absdiff(rgbImg, elem, res);
            return res;
            });
    }

Mat getMaxChannelsSum(const vector<Mat> &absdiffs)
    {
    return accumulate(absdiffs.begin(), absdiffs.end(), Mat(absdiffs[0].rows, absdiffs[0].cols, CV_32FC1, Scalar().all(0)),
        [](Mat& result, const Mat& elem) { max(result, sumChannels(elem), result); return result; });
    }

vector<Mat> getRBFOutput(const Mat& rgbImg, int t, double theta)
    {
    vector<Mat> neighbours;
    getAllUnitShifts(rgbImg, neighbours);

    // images of absolute differences between input pixels and its 8 neighbours
    vector<Mat> absdiffs;
    getAbsDiffs(rgbImg, neighbours, absdiffs);

    // output PSP 8 color images at time t
    vector<Mat> psp;
    transform(absdiffs.begin(), absdiffs.end(), back_inserter(psp),
        [t](const Mat& mat)
            {
            Mat tmat = Mat(mat.rows, mat.cols, mat.type(), Scalar().all(t));
            Mat res = Mat(mat.rows, mat.cols, mat.type(), Scalar().all(0));
            subtract(tmat, mat, res, tmat >= mat);
            return res;
            });

    vector<Mat> res;
    transform(psp.begin(), psp.end(), back_inserter(res),
        [theta](const Mat& mat)
            {
            return sumChannels(mat) > theta;
            });

    return res;
    }

} // namespace mpcnn
