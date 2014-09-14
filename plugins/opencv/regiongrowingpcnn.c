#include "regiongrowingpcnn.h"

void regionGrowingPcnn(const IplImage *grayImg, IplImage *output, double omega, double beta_min, double beta_max, double beta_delta, double d)
    {
    IplImage* G = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* L = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* U = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* T = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* P = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_8U, 1);
    IplImage* Y = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_8U, 1);

    cvConvertScale(grayImg, G, 1/255., 0);
    cvSetZero(P);

    /// Set \f$ d \f$ and \f$ \Omega \f$ 
    int t = 1;
    do
        {
        double wt_t = workingThreshold(G, P);
        threshold(P, omega, wt_t, T);
        pulseOutput(G, T, Y); // U = G
        double beta_t = beta_min; /// Set \f$ \beta_t \f$ sufficient for initial spread

        while (!statisticalTerminationConditionMet(beta_t, beta_max))
            {
            do
                {
                feeding(Y, L, d);
                linking(L, G, U, beta_t);
                }
            while (pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivity(U, T, Y));

            beta_t += beta_delta;
            }
        pulseMatrix(Y, P, t);
        ++t;
        }
    while (!allNeuronsHavePulsed(P));

    //TODO: Merge small regions with nearest neighbor.
    cvCopyImage(P, output);

    cvReleaseImage(&G);
    cvReleaseImage(&L);
    cvReleaseImage(&U);
    cvReleaseImage(&T);
    cvReleaseImage(&P);
    cvReleaseImage(&Y);
    }

double workingThreshold(const IplImage* G, const IplImage* P)
    {
    IplImage* mask = cvCreateImage(cvGetSize(P), IPL_DEPTH_8U, 1);
    double wt_t;
    cvCmpS(P, 0, mask, CV_CMP_EQ);
    cvMinMaxLoc(G, 0, &wt_t, 0, 0, mask);
    cvReleaseImage(&mask);
    return wt_t;
    }

void feeding(const IplImage* Y, IplImage* L, double d)
    {
    static float kernel[9] = {1, 1, 1, 1, 0, 1, 1, 1, 1};
    CvMat N = cvMat(sqrt(sizeof(kernel)/sizeof(kernel[0])), sqrt(sizeof(kernel)/sizeof(kernel[0])), CV_32F, kernel);
    cvFilter2D(Y, L, &N, cvPoint(-1,-1));
    cvSubS(L, cvRealScalar(d), L, 0);
    }

void linking(const IplImage* L, const IplImage* G, IplImage* U, double beta_t)
    {
    cvScale(L, U, beta_t, 0);
    cvAddS(U, cvRealScalar(1), U, 0);
    cvMul(G, U, U, 1.);
    }

void threshold(const IplImage* P, double omega, double wt_t, IplImage* T)
    {
    cvSet(T, cvRealScalar(wt_t), 0);
    cvSet(T, cvRealScalar(omega), P);
    }

void pulseOutput(const IplImage* U, const IplImage* T, IplImage* Y)
    {
    cvCmp(U, T, Y, CV_CMP_GE);
    cvScale(Y, Y, 1./255., 0.);
    }

int pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivity(const IplImage* U, const IplImage* T, IplImage* Y)
    {
    IplImage* change = (IplImage *)cvClone(Y);
    pulseOutput(U, T, Y);
    cvCmp(change, Y, change, CV_CMP_NE);
    double changeMaxVal;
    cvMinMaxLoc(change, 0, &changeMaxVal, 0, 0, 0);
    cvReleaseImage(&change);
    return changeMaxVal > 0;
    }

void pulseMatrix(const IplImage* Y, IplImage* P, int t)
    {
    IplImage* P_old = cvCreateImage(cvGetSize(Y), IPL_DEPTH_8U, 1);
    cvCmpS(Y, 0, P_old, CV_CMP_EQ);
    cvCopy(P, P_old, P_old);
    cvAddWeighted(Y, t, P_old, 1, 0, P);
    cvReleaseImage(&P_old);
    }

int allNeuronsHavePulsed(const IplImage* P)
    {
    double minVal;
    cvMinMaxLoc(P, &minVal, 0, 0, 0, 0);
    return minVal > 0;
    }

int statisticalTerminationConditionMet(double beta_t, double beta_max)
    {
    return (beta_t > beta_max); // or ..
    }
