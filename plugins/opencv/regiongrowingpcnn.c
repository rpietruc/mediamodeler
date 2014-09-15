#include "regiongrowingpcnn.h"

#ifndef ABS
#  define ABS(a) ((a) < 0 ? (-a) : (a))
#endif

static void print(const IplImage *P)
    {
    int i, j;
    for (i = 0; i < P->width; ++i)
        {
        for (j = 0; j < P->height; ++j)
            printf("%f ", cvGet2D(P, i, j).val[0]);
        printf("\n");
        }
    printf("\n");
    }

void regionGrowingPcnn(const IplImage *grayImg, IplImage *output, double omega, double beta_min, double beta_max, double beta_delta, double d, double SBmax)
    {
    IplImage* G = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* L = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* U = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* T = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* P = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_8U, 1);
    IplImage* Y = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_8U, 1);
    IplImage* Yold = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_8U, 1);

    cvConvertScale(grayImg, G, 1./255., 0);
    cvSetZero(P);

    /// Set \f$ d \f$ and \f$ \Omega \f$ 
    int t = 1;
    do
        {
        double wt_t = workingThreshold(G, P);
        threshold(P, omega, wt_t, T);
        pulseOutput(G, T, Y); // U = G
        double beta_t = beta_min; /// Set \f$ \beta_t \f$ sufficient for initial spread

        //cvCopy(Y, Yold, 0);
        //while (!statisticalTerminationConditionMet(G, Y, Yold, P, beta_t, beta_max, SBmax))
        do
            {
            cvCopy(Y, Yold, 0);
            do
                {
//                 printf("linking, feeding\n");
//                 print(Y);
                linking(Y, L, d);
                feeding(L, G, U, beta_t);
                }
            while (pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivity(U, T, Y));

//             printf("next\n");
            beta_t += beta_delta;
            }
        while (!statisticalTerminationConditionMet(G, Y, Yold, P, beta_t, beta_max, SBmax));

//         printf("pulse\n");
        pulseMatrix(Y, P, t);
//         print(P);
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
    cvReleaseImage(&Yold);
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

int excessiveBetaValue(double beta_t, double beta_max)
    {
    return (beta_t > beta_max);
    }

int regionEngulfed(const IplImage* Y, const IplImage* P)
    {
    int i, j, k, l;
    for (i = 0; i < Y->width; ++i)
        for (j = 0; j < Y->height; ++j)
            if (cvGet2D(Y, i, j).val[0] > 0)
                for (k = (i > 0 ? i - 1 : i); (k < Y->width) && (k <= (i + 1)); ++k)
                    for (l = (j > 0 ? j - 1 : j); (l < Y->height) && (l <= (j + 1)); ++l)
                        if (((k != i) || (l != j)) && (cvGet2D(Y, k, l).val[0]  < 1) && cvGet2D(P, k, l).val[0] < 1)
                            return 0;
    return 1;
    }

int excessiveMeanDifference(const IplImage* G, IplImage* Y, const IplImage* Yold, double SBmax)
    {
    int ret = 0;
    IplImage* Ynew = cvCreateImage(cvGetSize(Y), IPL_DEPTH_8U, 1);
    cvSub(Y, Yold, Ynew, Y);
    if (ABS(cvMean(G, Yold) - cvMean(G, Ynew)) > SBmax)
        {
        cvCopy(Yold, Y, 0); //if condition is violated the neuron is reset.
        ret = 1;
        }
    cvReleaseImage(&Ynew);
    return ret;
    }

int statisticalTerminationConditionMet(const IplImage* G, IplImage* Y, const IplImage* Yold, const IplImage* P, double beta_t, double beta_max, double SBmax)
    {
    return regionEngulfed(Y, P) || excessiveBetaValue(beta_t, beta_max) || excessiveMeanDifference(G, Y, Yold, SBmax);
    }
