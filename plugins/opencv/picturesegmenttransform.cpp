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

// Default parameters

PictureSegmentTransform::PictureSegmentTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("d", 1.);
    setProperty("Omega", 1);
    setProperty("initBeta", 0.2);
    setProperty("deltaBeta", 0.01);
    }

static bool allNeuronsHavePulsed(const IplImage* P)
    {
    double val;
    cvMinMaxLoc(P, &val, 0);
    return val != 0;
    }

/**
 * Threshold
\f[
    T_x[t] = \left\{ \begin{array}{l l}
        w t_t \text{,} & \quad \text{if $P_x[t - 1] = 0$} \\
        \Omega \text{,} & \quad \text{otherwise}
    \end{array} \right.
\f]
 */
void threshold(const IplImage* P, double omega, double wt_t, IplImage* T)
    {
    shared_ptr<IplImage> Wt_t(cvCreateImage(cvGetSize(P), IPL_DEPTH_32F, 1), IplImageDeleter());
    shared_ptr<IplImage> Omega(cvCreateImage(cvGetSize(P), IPL_DEPTH_32F, 1), IplImageDeleter());

    cvCmpS(P, 0, Wt_t.get(), CV_CMP_EQ);
    cvCmpS(P, 0, Omega.get(), CV_CMP_NE);
    cvAddWeighted(Wt_t.get(), wt_t, Omega.get(), omega, 0, T);
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

                QVector<float> kernel = (QVector<float>() << 1 << 1 << 1 << 1 << 0 << 1 << 1 << 1 << 1);
                CvMat N = cvMat(sqrt(kernel.size()), sqrt(kernel.size()), CV_32F, kernel.data());

                shared_ptr<IplImage> L(cvCreateImage(cvGetSize(G.get()), IPL_DEPTH_32F, 1), IplImageDeleter());
                shared_ptr<IplImage> U((IplImage *)cvClone(G.get()), IplImageDeleter());
                shared_ptr<IplImage> T(cvCreateImage(cvGetSize(G.get()), IPL_DEPTH_32F, 1), IplImageDeleter());
                shared_ptr<IplImage> P(cvCreateImage(cvGetSize(G.get()), IPL_DEPTH_32F, 1), IplImageDeleter());
                shared_ptr<IplImage> Y(cvCreateImage(cvGetSize(G.get()), 8, 1), IplImageDeleter());

                shared_ptr<IplImage> P_old(cvCreateImage(cvGetSize(G.get()), IPL_DEPTH_32F, 1), IplImageDeleter());

                /// Set \f$ \beta_t \f$ sufficient for initial spread
                int t = 1;
                do // while (!allNeuronsHavePulsed(P.get()))
                    {
                    /// \f$ wt_t = max[G_y] \f$.
                    double wt_t;
                    cvMinMaxLoc(G.get(), 0, &wt_t);

                    /// \f$ \farall y | P_y[t - 1] = 0 \f$.
                    //cvSetZero(P.get());

                    //cvCopyImage(G.get(), U.get()); // copied at initialization
                    //cvSet(T.get(), cvScalar(0));
                    threshold(P.get(), property("Omega").toDouble(), wt_t, T.get());

                    /**
                     * Pulse
                    \f[
                        Y_x[t] = \left\{ \begin{array}{l l}
                            1 \text{,} & \quad \text{if $G_x \geq T_x[t]$} \\
                            0 \text{,} & \quad \text{otherwise}
                        \end{array} \right.
                    \f]
                     */
                    cvCmp(G.get(), T.get(), Y.get(), CV_CMP_GE);

                    /// Set \f$ \beta_t \f$ sufficient for initial spread
                    double beta_t = property("initBeta").toDouble();

                    bool statisticalTerminationConditionMet = false;
                    while (!statisticalTerminationConditionMet)
                        {
                        bool thereIsAnyChangeInPulsingActivity = true;
                        while (thereIsAnyChangeInPulsingActivity)
                            {
                            /**
                             * Feeding: \f$ L_x[t] = \sum_{z \in N(x)} Y_z[t] - d \f$.
                             */
                            cvFilter2D(Y.get(), L.get(), &N);
                            cvSubS(L.get(), cvScalar(property("d").toDouble()), L.get());

                            /**
                             * Linking: \f$ U_x[t] = G_x\{1 + \beta_t L_x[t]\} \f$.
                             */
                            cvScale(L.get(), U.get(), beta_t);
                            cvAddS(U.get(), cvScalar(1), U.get());
                            cvMul(G.get(), U.get(), U.get());

                            /**
                             * Pulse output
                            \f[
                                Y_x[t] = \left\{ \begin{array}{l l}
                                    1 \text{,} & \quad \text{if $U_x[t] \geq T_x[t]$} \\
                                    0 \text{,} & \quad \text{otherwise}
                                \end{array} \right.
                            \f]
                             */
                            cvCmp(U.get(), T.get(), Y.get(), CV_CMP_GE);
                            }
                            // while (thereIsAnyChangeInPulsingActivity)
  
                            beta_t += property("deltaBeta").toDouble();
                        }
                        // while (!statisticalTerminationConditionMet)

                    /**
                     * Pulse matrix
                    \f[
                        P_x[t] = \left\{ \begin{array}{l l}
                            t \text{,} & \quad \text{if $Y_x[t] = 1$} \\
                            P_x[t - 1] \text{,} & \quad \text{otherwise}
                        \end{array} \right.
                    \f]
                     */
                    cvCmpS(Y.get(), 0, P_old.get(), CV_CMP_EQ);
                    cvAddWeighted(Y.get(), t, P_old.get(), 1, 0, P.get());
                    ++t;
                    }
                while (!allNeuronsHavePulsed(P.get()));

                // Merge small regions with nearest neighbor.

                // cvCopyImage(outputImg, mPictureFrame);
                cvConvertScale(Y.get(), mPictureFrame, 255.);

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
