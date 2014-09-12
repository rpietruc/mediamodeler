#include "pulsecoupledneuralnetwork.h"

void pcnn(const IplImage* grayImg, IplImage* output, float *m, int mSize, float *w, int wSize, int N, double alfa_F, double V_F, double alfa_L, double V_L, double beta, double alfa_T, double V_T)
    {
    IplImage* S = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* MY = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* WY = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* L = (IplImage *)cvClone(S);
    IplImage* F = (IplImage *)cvClone(S);
    IplImage* U = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* T = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
    IplImage* Y = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_8U, 1);

    CvMat M = cvMat(mSize, mSize, CV_32F, m);
    CvMat W = cvMat(wSize, wSize, CV_32F, w);

    cvConvertScale(grayImg, S, 1/255., 0.);
    cvSet(T, cvRealScalar(2), 0);
    cvSetZero(Y);

    int n;
    for (n = 0; n < N; ++n)
        {
        /* Feeding input: */
        cvFilter2D(Y, MY, &M, cvPoint(-1, -1));
        cvAddWeighted(F, exp(-alfa_F), MY, V_F, 0., F);
        cvAdd(F, S, F, 0);

        /* Linking input: */
        cvFilter2D(Y, WY, &W, cvPoint(-1, -1));
        cvAddWeighted(L, exp(-alfa_L), WY, V_L, 0., L);

        /* Linking: */
        cvScale(L, U, beta, 0.);
        cvAddS(U, cvRealScalar(1), U, 0);
        cvMul(F, U, U, 1.);

        /* Threshold: */
        cvAddWeighted(T, exp(-alfa_T), Y, V_T, 0., T);

        /* Pulse: */
        cvCmp(U, T, Y, CV_CMP_GE);
        }
    cvConvertScale(Y, output, 255., 0.);

    cvReleaseImage(&S);
    cvReleaseImage(&MY);
    cvReleaseImage(&WY);
    cvReleaseImage(&L);
    cvReleaseImage(&F);
    cvReleaseImage(&U);
    cvReleaseImage(&T);
    cvReleaseImage(&Y);
    }
