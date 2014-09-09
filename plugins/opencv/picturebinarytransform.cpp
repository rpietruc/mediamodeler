#include "picturebinarytransform.h"
#include "pictureframes.h"

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

static const double DefaultLinkingAmplification = 1;
static const double DefaultLinkingAttenuation = 0.6931;
static const QString DefaultLinkingKernel = "0.1091, 0.1409, 0.1091, 0.1409, 0, 0.1409, 0.1091, 0.1409, 0.1091";
static const double DefaultLinkingCoefficient = 0.2;

static const double DefaultThresholdAmplification = 20;
static const double DefaultThresholdAttenuation = 0.2;

class PulseBlock
    {
public:
    PulseBlock(const CvSize &aSize) :
        mOutputImg(cvCreateImage(aSize, 8, 1))
        {
        cvSetZero(mOutputImg);
        }

    virtual ~PulseBlock()
        {
        cvReleaseImage(&mOutputImg);
        }

    void process(const IplImage* aFeedingImg, const IplImage* aLinkingImg, const IplImage* aThresholdImg)
        {
        for(int i = 0; i < mOutputImg->height; ++i)
            for(int j = 0; j < mOutputImg->width; ++j)
                cvSet2D(mOutputImg, i, j, pulseFun(cvGet2D(aFeedingImg, i, j), cvGet2D(aLinkingImg, i, j), cvGet2D(aThresholdImg, i, j)));
        }

    const IplImage *getOutput() { return mOutputImg; }

protected:
    CvScalar pulseFun(const CvScalar &aFeeding, const CvScalar &aLinking, const CvScalar &aThreshold)
        {
        return cvScalar(aFeeding.val[0] * aFeeding.val[0] > aThreshold.val[0]);
        }

private:
    IplImage *mOutputImg;
    };

class FeedbackBlock
    {
public:
    FeedbackBlock(const CvSize &aSize) :
        mFeedbackImg(cvCreateImage(aSize, IPL_DEPTH_32F, 1))
        {}

    virtual ~FeedbackBlock()
        {
        cvReleaseImage(&mFeedbackImg);
        }

    void setFeedbackParams(double aAmplification, double aAttenuation)
        {
        mAmplification = aAmplification;
        mAttenuation = aAttenuation;
        }

    virtual IplImage *process(const IplImage* aInput)
        {
        for(int i = 0; i < mFeedbackImg->height; ++i)
            for(int j = 0; j < mFeedbackImg->width; ++j)
                cvSet2D(mFeedbackImg, i, j, cvScalar(exp(-mAttenuation) * cvGet2D(mFeedbackImg, i, j).val[0] + mAmplification * cvGet2D(aInput, i, j).val[0]));

        return mFeedbackImg;
        }

    const IplImage *getOutput() { return mFeedbackImg; }

protected:
    IplImage *mFeedbackImg;
    double mAmplification;
    double mAttenuation;
    };

class FilterBlock
    {
public:
    FilterBlock(const CvSize &aSize) :
        mFilteredImage(cvCreateImage(aSize, IPL_DEPTH_32F, 1))
        {}

    virtual ~FilterBlock()
        {
        cvReleaseImage(&mFilteredImage);
        }

    void setFilterParams(const CvMat &aKernel)
        {
        mKernel = aKernel;
        }

    virtual IplImage *process(const IplImage *aImg)
        {
        cvFilter2D(aImg, mFilteredImage, &mKernel);
        return mFilteredImage;
        }

    const IplImage *getOutput() { return mFilteredImage; }

    static CvMat textToKernel(const QString &aText)
        {
        QStringList list = aText.split(",", QString::SkipEmptyParts);
        QVector<float> kernel;
        foreach (const QString &s, list)
            kernel << s.toFloat();
        CvMat ret = cvMat(sqrt(kernel.size()), sqrt(kernel.size()), CV_32F, kernel.data());
        return ret;
        }

protected:
    IplImage *mFilteredImage;
    CvMat mKernel;
    };

class ThresholdBlock : public FeedbackBlock
    {
public:
    ThresholdBlock(const CvSize &aSize) :
        FeedbackBlock(aSize)
        {
        cvSet(mFeedbackImg, cvScalar(1));
        }
    };

class FeedingBlock : public FeedbackBlock, public FilterBlock
    {
public:
    FeedingBlock(const IplImage *aImg) :
        FeedbackBlock(cvGetSize(aImg)),
        FilterBlock(cvGetSize(aImg)),
        mSrcImg(cvCreateImage(cvGetSize(aImg), IPL_DEPTH_32F, 1))
        {
        cvCopyImage(aImg, mFeedbackImg);
        cvCopyImage(aImg, mSrcImg);
        }

    virtual ~FeedingBlock()
        {
        cvReleaseImage(&mSrcImg);
        }

    virtual IplImage *process(const IplImage *aImg)
        {
        FeedbackBlock::process(FilterBlock::process(aImg));
        for(int i = 0; i < mFeedbackImg->height; ++i)
            for(int j = 0; j < mFeedbackImg->width; ++j)
                cvSet2D(mFeedbackImg, i, j, cvScalar(cvGet2D(mFeedbackImg, i, j).val[0] + cvGet2D(mSrcImg, i, j).val[0]));

        return mFeedbackImg;
        }

    const IplImage *getOutput() { return mFeedbackImg; }

private:
    IplImage *mSrcImg;
    };

struct LinkingBlock : public FeedbackBlock, public FilterBlock
    {
public:
    LinkingBlock(const IplImage *aImg) :
        FeedbackBlock(cvGetSize(aImg)),
        FilterBlock(cvGetSize(aImg)),
        mOutputImg(cvCreateImage(cvGetSize(aImg), IPL_DEPTH_32F, 1))
        {
        cvCopyImage(aImg, mFeedbackImg);
        }

    void setLinkingParams(double aLinkingCoefficient) { mLinkingCoefficient = aLinkingCoefficient; }

    virtual IplImage *process(const IplImage *aImg)
        {
        FeedbackBlock::process(FilterBlock::process(aImg));
        for(int i = 0; i < mFeedbackImg->height; ++i)
            for(int j = 0; j < mFeedbackImg->width; ++j)
                cvSet2D(mOutputImg, i, j, cvScalar(1 + mLinkingCoefficient * cvGet2D(mFeedbackImg, i, j).val[0]));

        return mOutputImg;
        }

    const IplImage *getOutput() { return mOutputImg; }

private:
    double mLinkingCoefficient;
    IplImage *mOutputImg;
    };

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

                IplImage *grayImg = cvCreateImage(cvGetSize(srcFrame), 8, 1);
                cvCvtColor(srcFrame, grayImg, CV_BGR2GRAY);

                IplImage *inputImg = cvCreateImage(cvGetSize(grayImg), IPL_DEPTH_32F, 1);
                cvConvertScale(grayImg, inputImg, 1/255.);

                FeedingBlock feedingBlock(inputImg);
                LinkingBlock linkingBlock(inputImg);
                ThresholdBlock thresholdBlock(cvGetSize(inputImg));
                PulseBlock pulseBlock(cvGetSize(inputImg));

                feedingBlock.setFeedbackParams(property("feedingAmplification").toDouble(), property("feedingAttenuation").toDouble());
                feedingBlock.setFilterParams(FilterBlock::textToKernel(property("feedingKernel").toString()));

                linkingBlock.setFeedbackParams(property("linkingAmplification").toDouble(), property("linkingAttenuation").toDouble());
                linkingBlock.setFilterParams(FilterBlock::textToKernel(property("linkingKernel").toString()));
                linkingBlock.setLinkingParams(property("linkingCoefficient").toDouble());

                thresholdBlock.setFeedbackParams(property("thresholdAmplification").toDouble(), property("thresholdAttenuation").toDouble());

                for (int n = 0; n < property("iterations").toInt(); ++n)
                    {
                    feedingBlock.process(pulseBlock.getOutput());
                    linkingBlock.process(pulseBlock.getOutput());
                    thresholdBlock.process(pulseBlock.getOutput());
                    pulseBlock.process(feedingBlock.getOutput(), linkingBlock.getOutput(), thresholdBlock.getOutput());
                    }
                // cvCopyImage(outputImg, mPictureFrame);
                cvConvertScale(pulseBlock.getOutput(), mPictureFrame, 255.);

                cvReleaseImage(&grayImg);
                cvReleaseImage(&inputImg);

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
