#include "picturebinarytransform.h"
#include "pictureframes.h"
#include <boost/shared_ptr.hpp>

using namespace boost;

namespace media {

/**
 * Literature:
 * [Li14] Li, H., Jin, X., Yang, N., Yang, Z., The recognition of landed aircrafts based on PCNN model and affine moment invariants, Pattern Recognition Letters (2014)
 * 
 * PCNN structure according to [Li14]
 */

// Default parameters

//
// According to [Li14]
//
static const int DefaultIterations = 8;
static const double DefaultFeedingAmplification = 0;
static const double DefaultFeedingAttenuation = 0;
static const QString DefaultFeedingKernel = "0, 0, 0, 0, 0, 0, 0, 0, 0";

static const double DefaultLinkingAmplification = 1.;
static const double DefaultLinkingAttenuation = 0.6931;
static const QString DefaultLinkingKernel = "0.1091, 0.1409, 0.1091, 0.1409, 0, 0.1409, 0.1091, 0.1409, 0.1091";
static const double DefaultLinkingCoefficient = 0.2;

static const double DefaultThresholdAmplification = 20;
static const double DefaultThresholdAttenuation = 0.2;

PictureBinaryTransform::PictureBinaryTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("iterations", DefaultIterations);

    setProperty("feedingAmplification", DefaultFeedingAmplification);
    setProperty("feedingAttenuation", DefaultFeedingAttenuation);
    setProperty("feedingKernel", DefaultFeedingKernel);

    setProperty("linkingAmplification", DefaultLinkingAmplification);
    setProperty("linkingAttenuation", DefaultLinkingAttenuation);
    setProperty("linkingKernel", DefaultLinkingKernel);
    setProperty("linkingCoefficient", DefaultLinkingCoefficient);

    setProperty("thresholdAmplification", DefaultThresholdAmplification);
    setProperty("thresholdAttenuation", DefaultThresholdAttenuation);
    }

static QVector<float> text2vect(const QString& aText)
    {
    QStringList list = aText.split(",", QString::SkipEmptyParts);
    QVector<float> vect;
    foreach (const QString &s, list)
        vect << s.toFloat();
    return vect;
    }

void PictureBinaryTransform::process()
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

                shared_ptr<IplImage> inputImg(cvCreateImage(cvGetSize(srcFrame), IPL_DEPTH_32F, 1), IplImageDeleter());
                cvConvertScale(grayImg.get(), inputImg.get(), 1/255.);

                QVector<float> kernel = text2vect(property("feedingKernel").toString());
                CvMat feedingKernel = cvMat(sqrt(kernel.size()), sqrt(kernel.size()), CV_32F, kernel.data());
                shared_ptr<IplImage> feedingFilter(cvCreateImage(cvGetSize(srcFrame), IPL_DEPTH_32F, 1), IplImageDeleter());

                kernel = text2vect(property("linkingKernel").toString());
                CvMat linkingKernel = cvMat(sqrt(kernel.size()), sqrt(kernel.size()), CV_32F, kernel.data());
                shared_ptr<IplImage> linkingFilter(cvCreateImage(cvGetSize(srcFrame), IPL_DEPTH_32F, 1), IplImageDeleter());

                shared_ptr<IplImage> linkingInput((IplImage *)cvClone(inputImg.get()), IplImageDeleter());
                shared_ptr<IplImage> feedingInput((IplImage *)cvClone(inputImg.get()), IplImageDeleter());

                shared_ptr<IplImage> linking(cvCreateImage(cvGetSize(srcFrame), IPL_DEPTH_32F, 1), IplImageDeleter());

                shared_ptr<IplImage> thresholdFeedback(cvCreateImage(cvGetSize(srcFrame), IPL_DEPTH_32F, 1), IplImageDeleter());
                cvSet(thresholdFeedback.get(), cvScalar(2));

                shared_ptr<IplImage> pulseGenerator(cvCreateImage(cvGetSize(srcFrame), 8, 1), IplImageDeleter());
                cvSetZero(pulseGenerator.get());

                for (int n = 0; n < property("iterations").toInt(); ++n)
                    {
                    /**
                     * Feeding input:
                     * \f$ F_{i,j}[n] = e^{-\alpha_F} F_{i,j}[n - 1] + V_F \sum_{k,l} m_{i, j, k, l} Y_{i,j}[n - 1] + S_{i,j} \f$.
                     */
                    cvFilter2D(pulseGenerator.get(), feedingFilter.get(), &feedingKernel);
                    cvAddWeighted(feedingFilter.get(), exp(-property("feedingAttenuation").toDouble()), feedingInput.get(), property("feedingAmplification").toDouble(), 0., feedingFilter.get());
                    cvAdd(feedingInput.get(), inputImg.get(), feedingInput.get());

                    /**
                     * Linking input:
                     * \f$ L_{i,j}[n] = e^{-\alpha_L} L_{i,j}[n - 1] + V_L \sum_{k,l} w_{i, j, k, l} Y_{i,j}[n-1] \f$.
                     */
                    cvFilter2D(pulseGenerator.get(), linkingFilter.get(), &linkingKernel);
                    cvAddWeighted(linkingFilter.get(), exp(-property("linkingAttenuation").toDouble()), linkingInput.get(), property("linkingAmplification").toDouble(), 0., linkingFilter.get());

                    /**
                     * Linking:
                     * \f$ U_{i,j}[n] = F_{i,j}[n](1 + \beta L_{i,j}[n]) \f$.
                     */
                    cvScale(linkingInput.get(), linking.get(), property("linkingCoefficient").toFloat());
                    cvAddS(linking.get(), cvScalar(1), linking.get());
                    cvMul(feedingInput.get(), linking.get(), linking.get());

                    /**
                     * Threshold:
                     * \f$ T_{i,j}[n] = e^{-\alpha_T} T_{i,j}[n - 1] + V_T Y_{i,j}[n - 1] \f$.
                     */
                    cvAddWeighted(thresholdFeedback.get(), exp(-property("thresholdAttenuation").toDouble()), pulseGenerator.get(), property("thresholdAmplification").toDouble(), 0., thresholdFeedback.get());

                    /**
                     * Pulse:
                    \f[
                        Y_{i,j}[n] = \left\{ \begin{array}{l l}
                            1 & \quad U_{i,j}[n] \geq T_{i,j}[n] \\
                            0 & \quad \text{otherwise}
                        \end{array} \right.
                    \f]
                     */
                    cvCmp(linking.get(), thresholdFeedback.get(), pulseGenerator.get(), CV_CMP_GE);
                    }

                // cvCopyImage(outputImg, mPictureFrame);
                cvConvertScale(pulseGenerator.get(), mPictureFrame, 255.);

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
