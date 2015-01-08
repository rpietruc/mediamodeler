#include "regiongrowingpcnn.h"

#ifndef ABS
#  define ABS(a) ((a) < 0 ? (-a) : (a))
#endif

#if 0
#   include <stdio.h>
#   define LOGF(...) printf(__VA_ARGS__)
#else
#   define LOGF(...)
#endif

static void print(const IplImage *P, const char* name)
    {
    int i, j;
    LOGF("%s\n", name);
    for (i = 0; i < P->height; ++i)
        {
        for (j = 0; j < P->width; ++j)
            LOGF("%f ", cvGet2D(P, i, j).val[0]);
        LOGF("\n");
        }
    LOGF("\n");
    }

int equal(const IplImage* in1, const IplImage* in2, IplImage* out)
    {
    cvCmp(in1, in2, out, CV_CMP_NE);
    double outMaxVal;
    cvMinMaxLoc(out, 0, &outMaxVal, 0, 0, 0);
    return outMaxVal < 1;
    }

void normalize(IplImage* P)
    {
    double outMaxVal;
    cvMinMaxLoc(P, 0, &outMaxVal, 0, 0, 0);
    cvConvertScale(P, P, 255./outMaxVal, 0);
    }

void regionGrowingPcnn(const IplImage *grayImg, IplImage *P, double beta_min, double beta_max, double beta_delta, double d, double SBmax)
    {
    int t;
    double wt_t, beta_t;
    double omega = 10000;

    IplImage* G = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* L = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* U = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* T = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
//     IplImage* P = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_8U, 1);
    IplImage* Y = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_8U, 1);
    IplImage* Yold = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_8U, 1);
    IplImage* Ytmp = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_8U, 1);

    cvConvertScale(grayImg, G, 1./255., 0);
    print(G, "G");
//     cvSetZero(P);

    /// Set \f$ d \f$ and \f$ \Omega \f$ 
    for (t = 0; (t < 255) && !allNeuronsHavePulsed(P); pulseMatrix(Y, P, ++t))
        {                              print(P, "P");
        wt_t = workingThreshold(G, P); LOGF("wt_t = %f\n", wt_t);
        threshold(P, omega, wt_t, T);  print(T, "T");
        pulseOutput(G, T, Y);          print(Y, "Y");

        /// Set \f$ \beta_t \f$ sufficient for initial spread
        for (beta_t = beta_min; beta_t <= beta_max; beta_t  += beta_delta)
            {
            cvCopy(Y, Yold, 0);            LOGF("beta_t = %f\n", beta_t);
            int i;
            for (i = 0; i < sqrt(Y->imageSize); ++i)
                {
                linking(Y, L, d);              print(L, "L");
                feeding(L, G, U, beta_t);      print(U, "U");

                cvCopy(Y, Ytmp, 0);
                pulseOutput(U, T, Y);          print(Y, "Y");
                if (equal(Ytmp, Y, Ytmp))
                    break; //check if there was change in pulsing output
                }

            // check if any of statistical termination condition is met
            if (regionEngulfed(Y, P)) break;
            if ((beta_t > beta_min) && excessiveMeanDifference(G, Y, Yold, SBmax))
                {
                cvCopy(Yold, Y, 0);
                break;
                }
            }
        }

    //TODO: Merge small regions with nearest neighbor.
//     cvCopyImage(P, output);        print(P, "P");

    cvReleaseImage(&G);
    cvReleaseImage(&L);
    cvReleaseImage(&U);
    cvReleaseImage(&T);
//     cvReleaseImage(&P);
    cvReleaseImage(&Y);
    cvReleaseImage(&Yold);
    cvReleaseImage(&Ytmp);
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

void linking(const IplImage* Y, IplImage* L, double d)
    {
    static float kernel[9] = {1, 1, 1, 1, 0, 1, 1, 1, 1};
    CvMat N = cvMat(sqrt(sizeof(kernel)/sizeof(kernel[0])), sqrt(sizeof(kernel)/sizeof(kernel[0])), CV_32F, kernel);
    cvFilter2D(Y, L, &N, cvPoint(-1,-1));
    cvSubS(L, cvRealScalar(d), L, 0);
    }

void feeding(const IplImage* L, const IplImage* G, IplImage* U, double beta_t)
    {
    cvConvertScale(L, U, beta_t, 0);
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
    cvConvertScale(Y, Y, 1./255., 0.);
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

int regionEngulfed(const IplImage* Y, const IplImage* P)
    {
    int i, j, k, l;
    for (i = 0; i < Y->height; ++i)
        for (j = 0; j < Y->width; ++j)
            if (cvGet2D(Y, i, j).val[0] > 0)
                for (k = (i > 0 ? i - 1 : i); (k < Y->height) && (k <= (i + 1)); ++k)
                    for (l = (j > 0 ? j - 1 : j); (l < Y->width) && (l <= (j + 1)); ++l)
                        if (((k != i) || (l != j)) && (cvGet2D(Y, k, l).val[0]  < 1) && cvGet2D(P, k, l).val[0] < 1)
                            return 0;
    LOGF("region engulfed\n");
    return 1;
    }

int excessiveMeanDifference(const IplImage* G, const IplImage* Y, const IplImage* Yold, double SBmax)
    {
    int ret = 0;
    IplImage* Ynew = cvCreateImage(cvGetSize(Y), IPL_DEPTH_8U, 1);
    cvSub(Y, Yold, Ynew, Y);
    if (ABS(cvMean(G, Yold) - cvMean(G, Ynew)) > SBmax)
        {
        LOGF("excessiveMeanDifference\n");
        ret = 1;
        }
    cvReleaseImage(&Ynew);
    return ret;
    }
