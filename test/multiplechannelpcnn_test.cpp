#define BOOST_TEST_MODULE MultipleChannelPcnnTest
#include <boost/test/unit_test.hpp>
#include <boost/assign/std/vector.hpp>
#include <iostream>
#include "multiplechannelpcnn.h"

using namespace boost::assign;
using namespace std;
using namespace cv;
using namespace mpcnn;

BOOST_AUTO_TEST_CASE(getSeededPixelsTest)
    {
    vector<float> v;
    v += 1, 2, 3,
         4, 5, 6,
         7, 8, 9;

    vector< vector<float> > vtest;
    vector< vector<float> > vtest2;
    vtest.resize(v.size() - 1);
    vtest2.resize(v.size() - 1);

    vtest[0] += 5, 6, 0,
                8, 9, 0,
                0, 0, 0;
    vtest2[0] +=  4, 4, 3,
                  4, 4, 6,
                  7, 8, 9;

    vtest[1] += 2, 3, 0,
                5, 6, 0,
                8, 9, 0;
    vtest2[1] +=  1, 1, 3,
                  1, 1, 6,
                  1, 1, 9;

    vtest[2] += 0, 0, 0,
                2, 3, 0,
                5, 6, 0;
    vtest2[2] += 1, 2, 3,
                 2, 2, 6,
                 2, 2, 9;

    vtest[3] += 4, 5, 6,
                7, 8, 9,
                0, 0, 0;
    vtest2[3] += 3, 3, 3,
                 3, 3, 3,
                 7, 8, 9;

    vtest[4] += 0, 0, 0,
                1, 2, 3,
                4, 5, 6;
    vtest2[4] += 1, 2, 3,
                 3, 3, 3,
                 3, 3, 3;

    vtest[5] += 0, 4, 5,
                0, 7, 8,
                0, 0, 0;
    vtest2[5] += 1, 2, 2,
                 4, 2, 2,
                 7, 8, 9;

    vtest[6] += 0, 1, 2,
                0, 4, 5,
                0, 7, 8;
    vtest2[6] += 1, 1, 1,
                 4, 1, 1,
                 7, 1, 1;

    vtest[7] += 0, 0, 0,
                0, 1, 2,
                0, 4, 5;
    vtest2[7] += 1, 2, 3,
                 4, 4, 4,
                 7, 4, 4;

    vector<Mat> neighbours;
    getAllUnitShifts(Mat(v, true).reshape(1, 3), neighbours);

    BOOST_CHECK_EQUAL(neighbours.size(), v.size() - 1);
    for (int i = 0; i < (int)neighbours.size(); ++i)
        BOOST_CHECK_EQUAL(countNonZero(neighbours[i] == Mat(vtest[i], true).reshape(1, 3)), neighbours[i].total());

    vector<Mat> absdiffs;
    getAbsDiffs(Mat(v, true).reshape(1, 3), neighbours, absdiffs);
    for (int i = 0; i < (int)absdiffs.size(); ++i)
        BOOST_CHECK_EQUAL(countNonZero(absdiffs[i] == Mat(vtest2[i], true).reshape(1, 3)), absdiffs[i].total());

    vector<float> stest;
    stest += 4, 4, 3,
             4, 4, 6,
             7, 8, 9;
    BOOST_CHECK_EQUAL(countNonZero(getMaxChannelsSum(absdiffs) == Mat(stest, true).reshape(1, 3)), stest.size());

    vector<uchar> rtest;
    rtest += 255, 255, 255,
             255, 255,   0,
               0,   0,   0;
    Mat res = getSeededPixels(Mat(v, true).reshape(1, 3), 5);
    BOOST_CHECK(equal(res.begin<uchar>(), res.end<uchar>(), Mat(rtest, true).reshape(1, 3).begin<uchar>()));
    }

BOOST_AUTO_TEST_CASE(sumChannelsTest)
    {
    vector<float> v;
    v += 1,1,1, 2,2,2, 3,3,3,
         4,4,4, 5,5,5, 6,6,6;
    vector<float> vsum;
    vsum +=  3,  6,  9,
            12, 15, 18;
    Mat mat = Mat(v, true).reshape(3, 3);
    Mat channelsSum = sumChannels(mat);
    BOOST_CHECK(equal(channelsSum.begin<float>(), channelsSum.end<float>(), Mat(vsum, true).reshape(1, 3).begin<float>()));
    BOOST_CHECK(equal(channelsSum.begin<float>(), channelsSum.end<float>(), sumChannels(channelsSum).begin<float>()));
    }

BOOST_AUTO_TEST_CASE(getSeededPixelsColorTest)
    {
    vector<float> v;
    v += 1,1,1, 2,2,2, 3,3,3,
         4,4,4, 5,5,5, 6,6,6,
         7,7,7, 8,8,8, 9,9,9;

    vector< vector<float> > ntest;
    vector< vector<float> > dtest;
    ntest.resize(v.size() - 1);
    dtest.resize(v.size() - 1);
 
    ntest[0] += 5,5,5, 6,6,6, 0,0,0,
                8,8,8, 9,9,9, 0,0,0,
                0,0,0, 0,0,0, 0,0,0;
    dtest[0] +=     4,4,4, 4,4,4, 3,3,3,
                    4,4,4, 4,4,4, 6,6,6,
                    7,7,7, 8,8,8, 9,9,9;

    ntest[1] += 2,2,2, 3,3,3, 0,0,0,
                5,5,5, 6,6,6, 0,0,0,
                8,8,8, 9,9,9, 0,0,0;
    dtest[1] +=     1,1,1, 1,1,1, 3,3,3,
                    1,1,1, 1,1,1, 6,6,6,
                    1,1,1, 1,1,1, 9,9,9;

    ntest[2] += 0,0,0, 0,0,0, 0,0,0,
                2,2,2, 3,3,3, 0,0,0,
                5,5,5, 6,6,6, 0,0,0;
    dtest[2] +=     1,1,1, 2,2,2, 3,3,3,
                    2,2,2, 2,2,2, 6,6,6,
                    2,2,2, 2,2,2, 9,9,9;

    ntest[3] += 4,4,4, 5,5,5, 6,6,6,
                7,7,7, 8,8,8, 9,9,9,
                0,0,0, 0,0,0, 0,0,0;
    dtest[3] +=     3,3,3, 3,3,3, 3,3,3,
                    3,3,3, 3,3,3, 3,3,3,
                    7,7,7, 8,8,8, 9,9,9;

    ntest[4] += 0,0,0, 0,0,0, 0,0,0,
                1,1,1, 2,2,2, 3,3,3,
                4,4,4, 5,5,5, 6,6,6;
    dtest[4] +=     1,1,1, 2,2,2, 3,3,3,
                    3,3,3, 3,3,3, 3,3,3,
                    3,3,3, 3,3,3, 3,3,3;

    ntest[5] += 0,0,0, 4,4,4, 5,5,5,
                0,0,0, 7,7,7, 8,8,8,
                0,0,0, 0,0,0, 0,0,0;
    dtest[5] +=     1,1,1, 2,2,2, 2,2,2,
                    4,4,4, 2,2,2, 2,2,2,
                    7,7,7, 8,8,8, 9,9,9;

    ntest[6] += 0,0,0, 1,1,1, 2,2,2,
                0,0,0, 4,4,4, 5,5,5,
                0,0,0, 7,7,7, 8,8,8;
    dtest[6] +=     1,1,1, 1,1,1, 1,1,1,
                    4,4,4, 1,1,1, 1,1,1,
                    7,7,7, 1,1,1, 1,1,1;

    ntest[7] += 0,0,0, 0,0,0, 0,0,0,
                0,0,0, 1,1,1, 2,2,2,
                0,0,0, 4,4,4, 5,5,5;
    dtest[7] +=     1,1,1, 2,2,2, 3,3,3,
                    4,4,4, 4,4,4, 4,4,4,
                    7,7,7, 4,4,4, 4,4,4;

    vector<Mat> neighbours;
    getAllUnitShifts(Mat(v, true).reshape(3, 3), neighbours);

    BOOST_CHECK_EQUAL(neighbours.size(), v.size()/3 - 1);
    for (int i = 0; i < (int)neighbours.size(); ++i)
        BOOST_CHECK(equal(neighbours[i].begin<float>(), neighbours[i].end<float>(), Mat(ntest[i], true).reshape(3, 3).begin<float>()));

    vector<Mat> absdiffs;
    getAbsDiffs(Mat(v, true).reshape(3, 3), neighbours, absdiffs);
    for (int i = 0; i < (int)absdiffs.size(); ++i)
        BOOST_CHECK(equal(absdiffs[i].begin<float>(), absdiffs[i].end<float>(), Mat(dtest[i], true).reshape(3, 3).begin<float>()));

    vector<float> stest;
    stest += 12, 12,  9,
             12, 12, 18,
             21, 24, 27;
    BOOST_CHECK_EQUAL(countNonZero(getMaxChannelsSum(absdiffs) == Mat(stest, true).reshape(1, 3)), stest.size());

    vector<uchar> rtest;
    rtest += 255, 255, 255,
             255, 255,   0,
               0,   0,   0;
    Mat res = getSeededPixels(Mat(v, true).reshape(3, 3), 15);
    BOOST_CHECK(equal(res.begin<uchar>(), res.end<uchar>(), Mat(rtest, true).reshape(1, 3).begin<uchar>()));
    }

BOOST_AUTO_TEST_CASE(getRBFOutputTest)
    {
      
    }
