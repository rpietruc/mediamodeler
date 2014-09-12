#include "picturebinarytransform.h"
#include "pictureframes.h"
#include <boost/shared_ptr.hpp>
#include "pulsecoupledneuralnetwork.h"

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

                QVector<float> m = text2vect(property("feedingKernel").toString());
                QVector<float> w = text2vect(property("linkingKernel").toString());
                pcnn(grayImg.get(), mPictureFrame, m.data(), sqrt(m.size()), w.data(), sqrt(w.size()),
                     property("iterations").toInt(),
                     property("feedingAttenuation").toDouble(),
                     property("feedingAmplification").toDouble(),
                     property("linkingAttenuation").toDouble(),
                     property("linkingAmplification").toDouble(),
                     property("linkingCoefficient").toDouble(),
                     property("thresholdAttenuation").toDouble(),
                     property("thresholdAmplification").toDouble());

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
