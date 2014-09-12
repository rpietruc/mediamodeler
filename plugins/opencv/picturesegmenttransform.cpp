#include "picturesegmenttransform.h"
#include "pictureframes.h"
#include <boost/shared_ptr.hpp>

using namespace boost;

namespace media {

/**
  * Literature:
  * Robert D. Stewart, Iris Fermin, and Manfred Opper
  * Region Growing With Pulse-Coupled Neural Networks: An Alternative to Seeded Region Growing
  * IEEE TRANSACTIONS ON NEURAL NETWORKS, VOL. 13, NO. 6, NOVEMBER 2002
  */

PictureSegmentTransform::PictureSegmentTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    // Default parameters
    setProperty("d", 1.);
    setProperty("Omega", 1);
    setProperty("minBeta", 0.2);
    setProperty("deltaBeta", 0.01);
    setProperty("maxBeta", 1.);
    }

/**
  * Working threshold
  * \f$ wt_t = max[G_y] \quad \farall y | P_y[t - 1] = 0 \f$.
  * @todo optimize mask array allocation
  */
static double workingThreshold(const IplImage* G, const IplImage* P)
    {
    shared_ptr<IplImage> mask(cvCreateImage(cvGetSize(P), IPL_DEPTH_8U, 1), IplImageDeleter());
    double wt_t;
    cvCmpS(P, 0, mask.get(), CV_CMP_EQ);
    cvMinMaxLoc(G, 0, &wt_t, 0, 0, mask.get());
    return wt_t;
    }

/**
  * Feeding:
  * \f$ L_x[t] = \sum_{z \in N(x)} Y_z[t] - d \f$.
  */
static void feeding(const IplImage* Y, IplImage* L, double d)
    {
    QVector<float> kernel = (QVector<float>() << 1 << 1 << 1 << 1 << 0 << 1 << 1 << 1 << 1);
    CvMat N = cvMat(sqrt(kernel.size()), sqrt(kernel.size()), CV_32F, kernel.data());
    cvFilter2D(Y, L, &N);
    cvSubS(L, cvScalar(d), L);
    }

/** 
  * Linking:
  * \f$ U_x[t] = G_x\{1 + \beta_t L_x[t]\} \f$.
  */
static void linking(const IplImage* L, const IplImage* G, IplImage* U, double beta_t)
    {
    cvScale(L, U, beta_t);
    cvAddS(U, cvScalar(1), U);
    cvMul(G, U, U);
    }

/**
  * Threshold
  * \f[
  *     T_x[t] = \left\{ \begin{array}{l l}
  *         w t_t \text{,} & \quad \text{if $P_x[t - 1] = 0$} \\
  *         \Omega \text{,} & \quad \text{otherwise}
  *     \end{array} \right.
  * \f]
  */
static void threshold(const IplImage* P, double omega, double wt_t, IplImage* T)
    {
    cvSet(T, cvScalar(wt_t));
    cvSet(T, cvScalar(omega), P);
    }

/**
  * Pulse output
  * \f[
  *     Y_x[t] = \left\{ \begin{array}{l l}
  *         1 \text{,} & \quad \text{if $U_x[t] \geq T_x[t]$} \\
  *         0 \text{,} & \quad \text{otherwise}
  *     \end{array} \right.
  * \f]
  * 
  * @return true if there is any change in p ulsing activity
  * 
  * @todo optimize temporary array allocation
  */
static void pulseOutput(const IplImage* U, const IplImage* T, IplImage* Y)
    {
    cvCmp(U, T, Y, CV_CMP_GE);
    }

/**
  * pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivity
  * @return true if there is any change in p ulsing activity
  * @todo optimize temporary array allocation
  */
static bool pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivity(const IplImage* U, const IplImage* T, IplImage* Y)
    {
    shared_ptr<IplImage> change((IplImage *)cvClone(Y), IplImageDeleter());
    pulseOutput(U, T, Y);
    cvCmp(change.get(), Y, change.get(), CV_CMP_NE);
    double changeMaxVal;
    cvMinMaxLoc(change.get(), 0, &changeMaxVal);
    return changeMaxVal > 0;
    }

/**
  * Pulse matrix
  *  \f[
  *     P_x[t] = \left\{ \begin{array}{l l}
  *         t \text{,} & \quad \text{if $Y_x[t] = 1$} \\
  *         P_x[t - 1] \text{,} & \quad \text{otherwise}
  *     \end{array} \right.
  *  \f]
  *
  * @todo optimize temporary array allocation
  */
static void pulseMatrix(const IplImage* Y, IplImage* P, int t)
    {
    shared_ptr<IplImage> P_old(cvCreateImage(cvGetSize(Y), IPL_DEPTH_8U, 1), IplImageDeleter());
    cvCmpS(Y, 0, P_old.get(), CV_CMP_EQ);
    cvAddWeighted(Y, t, P_old.get(), 1, 0, P);
    }

static bool allNeuronsHavePulsed(const IplImage* P)
    {
    double minVal;
    cvMinMaxLoc(P, &minVal, 0);
    return minVal > 0;
    }

/**
  * statisticalTerminationConditionMet
  *
  * -# Region engulfed -- all neighbors of pulsing neurons are either pulsing or not active.
  * -# Excessive beta value -- the linking coefficient exceeds a maximum value \f$ \beta_{max} \f$.
  * -# Excessive mean difference -- difference \f$ \delta \mu \f$ between mean of accepted region \f$ \mu_{old} \f$
  *    and mean of newly proposed region \f$ \mu_{new} \f$ is greater than a threshold value \f$ S_{B_{max}} \f$.
  * 
  * @todo add 2 missing conditions
  */
static bool statisticalTerminationConditionMet(double beta_t, double beta_max)
    {
    return (beta_t > beta_max); // or ..
    }

void PictureSegmentTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (mPictureFrame.isCopyable(*frame))
                {
                PictureRGBFrame srcFrame;
                srcFrame.resizeAndCopyFrame(*frame);

                mPictureFrame.setSourceName(frame->getSourceName());
                mPictureFrame.resize(srcFrame.getDimensionT(PictureRGBFrame::Width).mResolution, srcFrame.getDimensionT(PictureRGBFrame::Height).mResolution);

                shared_ptr<IplImage> grayImg(cvCreateImage(cvGetSize(srcFrame), 8, 1), IplImageDeleter());
                cvCvtColor(srcFrame, grayImg.get(), CV_BGR2GRAY);

                shared_ptr<IplImage> G(cvCreateImage(cvGetSize(srcFrame), IPL_DEPTH_32F, 1), IplImageDeleter());
                cvConvertScale(grayImg.get(), G.get(), 1/255.);

                shared_ptr<IplImage> L(cvCreateImage(cvGetSize(G.get()), IPL_DEPTH_32F, 1), IplImageDeleter());
                shared_ptr<IplImage> U(cvCreateImage(cvGetSize(G.get()), IPL_DEPTH_32F, 1), IplImageDeleter());
                shared_ptr<IplImage> T(cvCreateImage(cvGetSize(G.get()), IPL_DEPTH_32F, 1), IplImageDeleter());
                shared_ptr<IplImage> P(cvCreateImage(cvGetSize(G.get()), IPL_DEPTH_8U, 1), IplImageDeleter());
                shared_ptr<IplImage> Y(cvCreateImage(cvGetSize(G.get()), IPL_DEPTH_8U, 1), IplImageDeleter());
                cvSetZero(P.get());

                /// Set \f$ d \f$ and \f$ \Omega \f$ 
                int t = 1;
                do
                    {
                    double wt_t = workingThreshold(G.get(), P.get());
                    threshold(P.get(), property("Omega").toDouble(), wt_t, T.get());
                    pulseOutput(G.get(), T.get(), Y.get()); // U = G
                    double beta_t = property("minBeta").toDouble(); /// Set \f$ \beta_t \f$ sufficient for initial spread

                    while (!statisticalTerminationConditionMet(beta_t, property("maxBeta").toDouble()))
                        {
                        do
                            {
                            feeding(Y.get(), L.get(), property("d").toDouble());
                            linking(L.get(), G.get(), U.get(), beta_t);
                            }
                        while (pulseOutputAndCheckIfThereIsAnyChangeInPulsingActivity(U.get(), T.get(), Y.get()));

                        beta_t += property("deltaBeta").toDouble();
                        }
                    pulseMatrix(Y.get(), P.get(), t);
                    ++t;
                    }
                while (!allNeuronsHavePulsed(P.get()));

                // Merge small regions with nearest neighbor.

                // cvCopyImage(outputImg, mPictureFrame);
                cvConvertScale(P.get(), mPictureFrame, 255.);

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
