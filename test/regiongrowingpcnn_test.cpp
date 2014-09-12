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

CvSize size = cvSize(2, 2);

BOOST_AUTO_TEST_CASE(workingThresholdTest)
    {
    shared_ptr<IplImage> P(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> G(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
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

/**
  * Feeding:
  * \f$ L_x[t] = \sum_{z \in N(x)} Y_z[t] - d \f$.
  */
BOOST_AUTO_TEST_CASE(feedingTest)
    {
    shared_ptr<IplImage> Y(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> L(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    cvSet2D(Y.get(), 0, 0, cvRealScalar(1));
    cvSet2D(Y.get(), 0, 1, cvRealScalar(2));
    cvSet2D(Y.get(), 1, 0, cvRealScalar(3));
    cvSet2D(Y.get(), 1, 1, cvRealScalar(4));

    feeding(Y.get(), L.get(), 1);

    BOOST_CHECK_EQUAL(cvGet2D(L.get(), 0, 0).val[0], 8);
    BOOST_CHECK_EQUAL(cvGet2D(L.get(), 0, 1).val[0], 7);
    BOOST_CHECK_EQUAL(cvGet2D(L.get(), 1, 0).val[0], 6);
    BOOST_CHECK_EQUAL(cvGet2D(L.get(), 1, 1).val[0], 5);
    }

BOOST_AUTO_TEST_CASE(pulseOutputTest)
    {
    shared_ptr<IplImage> U(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
    shared_ptr<IplImage> T(cvCreateImage(size, IPL_DEPTH_8U, 1), IplImageDeleter());
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
