#define BOOST_TEST_MODULE EllipseMergeTest
#include <boost/test/unit_test.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/shared_ptr.hpp>
#include "regiongrowingpcnn.h"

using namespace boost;

struct IplImageDeleter
    {
    void operator() (IplImage* aImage)
        {
        cvReleaseImage(&aImage);
        }
    };

BOOST_AUTO_TEST_CASE(workingThresholdTest)
    {
    CvSize size = cvSize(2, 2);

    shared_ptr<IplImage> P(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> G(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());
    cvSet2D(P.get(), 0, 0, cvRealScalar(0));
    cvSet2D(P.get(), 0, 1, cvRealScalar(0));
    cvSet2D(P.get(), 1, 0, cvRealScalar(0));
    cvSet2D(P.get(), 1, 1, cvRealScalar(1));

    cvSet2D(G.get(), 0, 0, cvRealScalar(3));
    cvSet2D(G.get(), 0, 1, cvRealScalar(1));
    cvSet2D(G.get(), 1, 0, cvRealScalar(2));
    cvSet2D(G.get(), 1, 1, cvRealScalar(4));

    BOOST_CHECK_EQUAL(workingThreshold(G.get(), P.get()), 3);
    }

BOOST_AUTO_TEST_CASE(linkingTest)
    {
    CvSize size = cvSize(3, 3);

    shared_ptr<IplImage> Y(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> L(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());

    // 1 2 3
    // 4 0 5
    // 6 7 8
    cvSet2D(Y.get(), 0, 0, cvRealScalar(0));
    cvSet2D(Y.get(), 0, 1, cvRealScalar(1));
    cvSet2D(Y.get(), 0, 2, cvRealScalar(0));
    cvSet2D(Y.get(), 1, 0, cvRealScalar(1));
    cvSet2D(Y.get(), 1, 1, cvRealScalar(0));
    cvSet2D(Y.get(), 1, 2, cvRealScalar(1));
    cvSet2D(Y.get(), 2, 0, cvRealScalar(0));
    cvSet2D(Y.get(), 2, 1, cvRealScalar(1));
    cvSet2D(Y.get(), 2, 2, cvRealScalar(0));

    linking(Y.get(), L.get(), 0.5);

    BOOST_CHECK_EQUAL(cvGet2D(L.get(), 1, 1).val[0], 3.5);
    }

BOOST_AUTO_TEST_CASE(feedingTest)
    {
    CvSize size = cvSize(2, 2);
    double beta_t = 0.5;

    shared_ptr<IplImage> L(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());
    shared_ptr<IplImage> G(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());
    shared_ptr<IplImage> U(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());

    cvSet2D(L.get(), 0, 0, cvRealScalar(0));
    cvSet2D(L.get(), 0, 1, cvRealScalar(1));
    cvSet2D(L.get(), 1, 0, cvRealScalar(2));
    cvSet2D(L.get(), 1, 1, cvRealScalar(3));
    cvSet(G.get(), cvRealScalar(2));

    feeding(L.get(), G.get(), U.get(), beta_t);

    BOOST_CHECK_EQUAL(cvGet2D(U.get(), 0, 0).val[0], 2);
    BOOST_CHECK_EQUAL(cvGet2D(U.get(), 0, 1).val[0], 3);
    BOOST_CHECK_EQUAL(cvGet2D(U.get(), 1, 0).val[0], 4);
    BOOST_CHECK_EQUAL(cvGet2D(U.get(), 1, 1).val[0], 5);
    }

BOOST_AUTO_TEST_CASE(thresholdTest)
    {
    CvSize size = cvSize(2, 2);

    double omega = 0.1;
    double wt_t = 0.2;

    shared_ptr<IplImage> P(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> T(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());

    cvSet2D(P.get(), 0, 0, cvRealScalar(0));
    cvSet2D(P.get(), 0, 1, cvRealScalar(1));
    cvSet2D(P.get(), 1, 0, cvRealScalar(2));
    cvSet2D(P.get(), 1, 1, cvRealScalar(3));

    threshold(P.get(), omega, wt_t, T.get());

    double epsilon = 0.0001;
    BOOST_CHECK_CLOSE(cvGet2D(T.get(), 0, 0).val[0], wt_t, epsilon);
    BOOST_CHECK_CLOSE(cvGet2D(T.get(), 0, 1).val[0], omega, epsilon);
    BOOST_CHECK_CLOSE(cvGet2D(T.get(), 1, 0).val[0], omega, epsilon);
    BOOST_CHECK_CLOSE(cvGet2D(T.get(), 1, 1).val[0], omega, epsilon);
    }

BOOST_AUTO_TEST_CASE(pulseOutputTest)
    {
    CvSize size = cvSize(2, 2);

    shared_ptr<IplImage> U(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());
    shared_ptr<IplImage> T(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());
    shared_ptr<IplImage> Y(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());

    cvSet2D(U.get(), 0, 0, cvRealScalar(0));
    cvSet2D(U.get(), 0, 1, cvRealScalar(1));
    cvSet2D(U.get(), 1, 0, cvRealScalar(2));
    cvSet2D(U.get(), 1, 1, cvRealScalar(3));
    cvSet(T.get(), cvRealScalar(2));

    // U >= T
    pulseOutput(U.get(), T.get(), Y.get());

    BOOST_CHECK_EQUAL(cvGet2D(Y.get(), 0, 0).val[0], 0);
    BOOST_CHECK_EQUAL(cvGet2D(Y.get(), 0, 1).val[0], 0);
    BOOST_CHECK_EQUAL(cvGet2D(Y.get(), 1, 0).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(Y.get(), 1, 1).val[0], 1);
    }

BOOST_AUTO_TEST_CASE(changeInPulsingActivityTest)
    {
    CvSize size = cvSize(2, 2);

    shared_ptr<IplImage> U(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());
    shared_ptr<IplImage> T(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());
    shared_ptr<IplImage> Y(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> Ytmp(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());

    cvSet2D(U.get(), 0, 0, cvRealScalar(0));
    cvSet2D(U.get(), 0, 1, cvRealScalar(1));
    cvSet2D(U.get(), 1, 0, cvRealScalar(2));
    cvSet2D(U.get(), 1, 1, cvRealScalar(3));
    cvSet(T.get(), cvRealScalar(2));

    cvSet(Y.get(), cvRealScalar(0));

    // pulse and check if there was change in pulsing output
    cvCopy(Y.get(), Ytmp.get(), 0);
    pulseOutput(U.get(), T.get(), Y.get());
    BOOST_CHECK_EQUAL(equal(Ytmp.get(), Y.get(), Ytmp.get()), 0);

    // pulse and check if there was change in pulsing output
    cvCopy(Y.get(), Ytmp.get(), 0);
    pulseOutput(U.get(), T.get(), Y.get());
    BOOST_CHECK_EQUAL(equal(Ytmp.get(), Y.get(), Ytmp.get()), 1);

    BOOST_CHECK_EQUAL(cvGet2D(Y.get(), 0, 0).val[0], 0);
    BOOST_CHECK_EQUAL(cvGet2D(Y.get(), 0, 1).val[0], 0);
    BOOST_CHECK_EQUAL(cvGet2D(Y.get(), 1, 0).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(Y.get(), 1, 1).val[0], 1);
    }

BOOST_AUTO_TEST_CASE(pulseMatrixTest)
    {
    CvSize size = cvSize(2, 2);
 
    shared_ptr<IplImage> Y(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> P(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());

    cvSetZero(Y.get());
    cvSet2D(Y.get(), 1, 1, cvRealScalar(1));

    cvSet2D(P.get(), 0, 0, cvRealScalar(0));
    cvSet2D(P.get(), 0, 1, cvRealScalar(1));
    cvSet2D(P.get(), 1, 0, cvRealScalar(2));
    cvSet2D(P.get(), 1, 1, cvRealScalar(3));

    int t = 5;
    pulseMatrix(Y.get(), P.get(), t);

    BOOST_CHECK_EQUAL(cvGet2D(P.get(), 0, 0).val[0], 0);
    BOOST_CHECK_EQUAL(cvGet2D(P.get(), 0, 1).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(P.get(), 1, 0).val[0], 2);
    BOOST_CHECK_EQUAL(cvGet2D(P.get(), 1, 1).val[0], 5);
    }

BOOST_AUTO_TEST_CASE(allNeuronsHavePulsedTest)
    {
    CvSize size = cvSize(2, 2);

    shared_ptr<IplImage> P(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());

    cvSet2D(P.get(), 0, 0, cvRealScalar(0));
    cvSet2D(P.get(), 0, 1, cvRealScalar(1));
    cvSet2D(P.get(), 1, 0, cvRealScalar(2));
    cvSet2D(P.get(), 1, 1, cvRealScalar(3));

    BOOST_CHECK_EQUAL(allNeuronsHavePulsed(P.get()), 0);
    cvSet2D(P.get(), 0, 0, cvRealScalar(4));
    BOOST_CHECK_EQUAL(allNeuronsHavePulsed(P.get()), 1);
    }

BOOST_AUTO_TEST_CASE(regionEngulfedTest)
    {
    CvSize size = cvSize(2, 2);
    shared_ptr<IplImage> P(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> Y(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());

    cvSet2D(P.get(), 0, 0, cvRealScalar(0));
    cvSet2D(P.get(), 0, 1, cvRealScalar(1));
    cvSet2D(P.get(), 1, 0, cvRealScalar(2));
    cvSet2D(P.get(), 1, 1, cvRealScalar(3));

    cvSet2D(Y.get(), 0, 0, cvRealScalar(1));
    cvSet2D(Y.get(), 0, 1, cvRealScalar(1));
    cvSet2D(Y.get(), 1, 0, cvRealScalar(1));
    cvSet2D(Y.get(), 1, 1, cvRealScalar(1));

    BOOST_CHECK_EQUAL(regionEngulfed(Y.get(), P.get()), 1);

    cvSet2D(Y.get(), 0, 0, cvRealScalar(0));
    BOOST_CHECK_EQUAL(regionEngulfed(Y.get(), P.get()), 0);

    cvSet2D(P.get(), 0, 0, cvRealScalar(4));
    BOOST_CHECK_EQUAL(regionEngulfed(Y.get(), P.get()), 1);
    }

void createExampleImg(IplImage* I)
    {
    // FFEEBB77  // 1113
    // CCBBAA88  // 1112
    // DDEE88AA  // 1122
    // CCDD99CC  // 1122
    // FF/FF = 1.
    // EE/FF = .9(3)
    // DD/FF = .8(6)
    // CC/FF = .8
    // BB/FF = .7(3)
    // AA/FF = .(6)
    // 99/FF = .6
    // 88/FF = .5(3)

    cvSet2D(I, 0, 0, cvRealScalar(0xFF));
    cvSet2D(I, 0, 1, cvRealScalar(0xEE));
    cvSet2D(I, 0, 2, cvRealScalar(0xBB));
    cvSet2D(I, 0, 3, cvRealScalar(0x77));

    cvSet2D(I, 1, 0, cvRealScalar(0xCC));
    cvSet2D(I, 1, 1, cvRealScalar(0xBB));
    cvSet2D(I, 1, 2, cvRealScalar(0xAA));
    cvSet2D(I, 1, 3, cvRealScalar(0x88));

    cvSet2D(I, 2, 0, cvRealScalar(0xDD));
    cvSet2D(I, 2, 1, cvRealScalar(0xEE));
    cvSet2D(I, 2, 2, cvRealScalar(0x88));
    cvSet2D(I, 2, 3, cvRealScalar(0xAA));

    cvSet2D(I, 3, 0, cvRealScalar(0xCC));
    cvSet2D(I, 3, 1, cvRealScalar(0xDD));
    cvSet2D(I, 3, 2, cvRealScalar(0x99));
    cvSet2D(I, 3, 3, cvRealScalar(0xCC));
    }

BOOST_AUTO_TEST_CASE(excessiveMeanDifferenceTest)
    {
    CvSize size = cvSize(4, 4);
    double SBmax = 0.13;

    shared_ptr<IplImage> G(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());
    shared_ptr<IplImage> Y(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> Yold(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    createExampleImg(G.get());
    cvSetZero(Y.get());
    cvRectangle(Y.get(), cvPoint(0, 0), cvPoint(1, 3), cvRealScalar(1.), CV_FILLED, 8, 0);
    cvCopy(Y.get(), Yold.get());
    cvSet2D(Y.get(), 0, 2, cvRealScalar(1.));
    // FFEE + BB
    // CCBB
    // DDEE
    // CCDD
    // .8(6) - .7(3) = .1(3)
    BOOST_CHECK_EQUAL(excessiveMeanDifference(G.get(), Y.get(), Yold.get(), SBmax), 1);
    }

BOOST_AUTO_TEST_CASE(regionGrowingPcnnTest)
    {
    double epsilon = 0.0001;
    CvSize size = cvSize(4, 4);
    shared_ptr<IplImage> I(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> O(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());

    createExampleImg(I.get());

    double omega = 1000;
    double beta_min = 0.2;
    double beta_max = 1.;
    double beta_delta = 0.01;
    double d = 0.5;
    double SBmax = 0.13;

    IplImage* G = cvCreateImage(size, IPL_DEPTH_32F, 1);
    IplImage* L = cvCreateImage(size, IPL_DEPTH_32F, 1);
    IplImage* U = cvCreateImage(size, IPL_DEPTH_32F, 1);
    IplImage* T = cvCreateImage(size, IPL_DEPTH_32F, 1);
    IplImage* P = cvCreateImage(size, IPL_DEPTH_8U, 1);
    IplImage* Y = cvCreateImage(size, IPL_DEPTH_8U, 1);
    cvConvertScale(I.get(), G, 1./255., 0);
    cvSetZero(P);

//    int t = 1;
    double wt_t = workingThreshold(G, P);
    BOOST_CHECK_CLOSE(wt_t, 1., epsilon);
    threshold(P, omega, wt_t, T);
    for (int i = 0; i < size.height; ++i)
        for (int j = 0; j < size.width; ++j)
            BOOST_CHECK_CLOSE(cvGet2D(T, i, j).val[0], 1., epsilon);

    // 1000
    // 0000
    // 0000
    // 0000
    pulseOutput(G, T, Y); // U = G
    for (int i = 0; i < size.height; ++i)
        for (int j = 0; j < size.width; ++j)
            if (i+j)
                BOOST_CHECK_CLOSE(cvGet2D(Y, i, j).val[0], 0., epsilon);
            else
                BOOST_CHECK_CLOSE(cvGet2D(Y, i, j).val[0], 1., epsilon);

//    double beta_t = beta_min;
    linking(Y, L, d);
    BOOST_CHECK_CLOSE(cvGet2D(L, 1, 1).val[0], 1. - d, epsilon);
    BOOST_CHECK_CLOSE(cvGet2D(L, 1, 2).val[0], -d, epsilon);
    BOOST_CHECK_CLOSE(cvGet2D(L, 2, 1).val[0], -d, epsilon);
    BOOST_CHECK_CLOSE(cvGet2D(L, 2, 2).val[0], -d, epsilon);

    cvReleaseImage(&G);
    cvReleaseImage(&L);
    cvReleaseImage(&U);
    cvReleaseImage(&T);
    cvReleaseImage(&P);
    cvReleaseImage(&Y);

    regionGrowingPcnn(I.get(), O.get(), omega, beta_min, beta_max, beta_delta, d, SBmax);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 0, 0).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 0, 1).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 0, 2).val[0], 2);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 0, 3).val[0], 2);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 1, 0).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 1, 1).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 1, 2).val[0], 2);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 1, 3).val[0], 2);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 2, 0).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 2, 1).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 2, 2).val[0], 2);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 2, 3).val[0], 2);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 3, 0).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 3, 1).val[0], 1);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 3, 2).val[0], 2);
    BOOST_CHECK_EQUAL(cvGet2D(O.get(), 3, 3).val[0], 2);
    }
