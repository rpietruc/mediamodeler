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

BOOST_AUTO_TEST_CASE(feedingTest)
    {
    CvSize size = cvSize(3, 3);

    shared_ptr<IplImage> Y(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> L(cvCreateImage(size, IPL_DEPTH_32F, 1), IplImageDeleter());

    // 1 2 3
    // 4 0 5
    // 6 7 8
    cvSet2D(Y.get(), 0, 0, cvRealScalar(1));
    cvSet2D(Y.get(), 0, 1, cvRealScalar(2));
    cvSet2D(Y.get(), 0, 2, cvRealScalar(3));
    cvSet2D(Y.get(), 1, 0, cvRealScalar(4));
    cvSet2D(Y.get(), 1, 1, cvRealScalar(0));
    cvSet2D(Y.get(), 1, 2, cvRealScalar(5));
    cvSet2D(Y.get(), 2, 0, cvRealScalar(6));
    cvSet2D(Y.get(), 2, 1, cvRealScalar(7));
    cvSet2D(Y.get(), 2, 2, cvRealScalar(8));

    feeding(Y.get(), L.get(), 1);

    BOOST_CHECK_EQUAL(cvGet2D(L.get(), 1, 1).val[0], 35);
    }

BOOST_AUTO_TEST_CASE(linkingTest)
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

    linking(L.get(), G.get(), U.get(), beta_t);

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

BOOST_AUTO_TEST_CASE(pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivityTest)
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

    cvSet(Y.get(), cvRealScalar(0));
    int res = pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivity(U.get(), T.get(), Y.get());
    BOOST_CHECK_EQUAL(res, 1);
    res = pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivity(U.get(), T.get(), Y.get());
    BOOST_CHECK_EQUAL(res, 0);

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

    cvSet2D(Y.get(), 0, 0, cvRealScalar(0));
    cvSet2D(Y.get(), 0, 1, cvRealScalar(1));
    cvSet2D(Y.get(), 1, 0, cvRealScalar(2));
    cvSet2D(Y.get(), 1, 1, cvRealScalar(3));

    int t = 5;
    pulseMatrix(Y.get(), P.get(), t);

    BOOST_CHECK_EQUAL(cvGet2D(P.get(), 0, 0).val[0], 0);
    BOOST_CHECK_EQUAL(cvGet2D(P.get(), 0, 1).val[0], t);
    BOOST_CHECK_EQUAL(cvGet2D(P.get(), 1, 0).val[0], 2);
    BOOST_CHECK_EQUAL(cvGet2D(P.get(), 1, 1).val[0], 3);
    }
