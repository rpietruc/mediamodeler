#include "picturesegmenttransform.h"
#include "pictureframes.h"
#include <boost/shared_ptr.hpp>
#include "pulsecoupledneuralnetwork.h"

using namespace boost;

namespace media {

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
