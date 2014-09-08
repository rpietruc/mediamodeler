#include "picturebinarytransform.h"
#include "pictureframes.h"

namespace media {

void PCNNTransform(IplImage *aImg, IplImage *aResult, const CvMat &aKernelF, const CvMat &aKernelL,
    int aIters = 8,
    double aV_F = 0.01,
    double aV_L = 1,
    double aV_T = 0.2,
    double aAlfa_F = 0.693,
    double aAlfa_L = 0.693,
    double aAlfa_T = 0.069,
    double aBeta = 0.2);

PictureBinaryTransform::PictureBinaryTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("iterations", 8);
    setProperty("feedbackAmplification", 0.01);
    setProperty("linkingAmplification", 1);
    setProperty("thresholdAmplification", 0.2);
    setProperty("feedbackAttenuation", 0.693);
    setProperty("linkingAttenuation", 0.693);
    setProperty("thresholdAttenuation", 0.069);
    setProperty("linkingCoefficient", 0.2);
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

                float kF[3][3] = {
                    {0.707, 1, 0.707},
                    {1, 1, 1},
                    {0.707, 1, 0.707}};

                float kL[3][3] = {
                    {0.1091, 0.1409, 0.1091},
                    {0.1409, 0, 0.1409},
                    {0.1091, 0.1409, 0.1091}};

                CvMat kernelL = cvMat(3, 3, CV_32F, kL);
                CvMat kernelF = cvMat(3, 3, CV_32F, kF);

                PCNNTransform(srcFrame, mPictureFrame, kernelF, kernelL, 
                    property("iterations").toInt(),
                    property("feedbackAmplification").toDouble(),
                    property("linkingAmplification").toDouble(),
                    property("thresholdAmplification").toDouble(),
                    property("feedbackAttenuation").toDouble(),
                    property("linkingAttenuation").toDouble(),
                    property("thresholdAttenuation").toDouble(),
                    property("linkingCoefficient").toDouble());
                emit framesReady();
                break;
                }
            }
    }

/**
 * source: http://www.oschina.net/code/piece_full?code=5828
 */
void PCNNTransform(IplImage *aImg, IplImage *aResult, const CvMat &aKernelF, const CvMat &aKernelL,
    int aIters, double aV_F, double aV_L, double aV_T, double aAlfa_F, double aAlfa_L, double aAlfa_T, double aBeta)
    {
    IplImage *imgGray = cvCreateImage(cvGetSize(aImg),8,1);
    IplImage *kL = cvCreateImage(cvGetSize(aImg), IPL_DEPTH_32F, 1);
    IplImage *kF = cvCreateImage(cvGetSize(aImg), IPL_DEPTH_32F, 1);
    IplImage *S  = cvCreateImage(cvGetSize(aImg), IPL_DEPTH_32F, 1);
    IplImage *F  = cvCreateImage(cvGetSize(aImg), IPL_DEPTH_32F, 1);
    IplImage *L  = cvCreateImage(cvGetSize(aImg), IPL_DEPTH_32F, 1);
    IplImage *T  = cvCreateImage(cvGetSize(aImg), IPL_DEPTH_32F, 1);
    IplImage *Y  = cvCreateImage(cvGetSize(aImg), IPL_DEPTH_32F, 1);

    cvCvtColor(aImg, imgGray, CV_BGR2GRAY);
    for (int i = 0; i < S->height; ++i)
        for (int j = 0; j < S->width; ++j)
            cvSet2D(S, i, j, cvScalar(cvGet2D(imgGray, i, j).val[0]/255.0, cvGet2D(imgGray, i, j).val[1]/255.0, cvGet2D(imgGray, i, j).val[2]/255.0));

    cvSetZero(Y);
    cvSet(T, cvScalar(2));

    cvCopyImage(S, F);
    cvCopyImage(S, L);
 
    for (int n = 0; n < aIters; ++n)
        {
        cvFilter2D(Y, kL, &aKernelL);
        cvFilter2D(Y, kF, &aKernelF);

        for(int i = 0; i < S->height; ++i)
            for(int j = 0; j < S->width; ++j)
                {
                //aV_L = cvGet2D(L, i, j).val[0];
                cvSet2D(F, i, j, cvScalar(exp(-aAlfa_F) * cvGet2D(F, i, j).val[0] + aV_F * cvGet2D(kF, i, j).val[0] + cvGet2D(S, i, j).val[0]));
                cvSet2D(L, i, j, cvScalar(exp(-aAlfa_L) * cvGet2D(L, i, j).val[0] + aV_L * cvGet2D(kL, i, j).val[0]));
                cvSet2D(T, i, j, cvScalar(exp(-aAlfa_T) * cvGet2D(T, i, j).val[0] + aV_T * cvGet2D(Y, i, j).val[0]));
                cvSet2D(Y, i, j, cvScalar((cvGet2D(F, i, j).val[0] * (1 + aBeta * cvGet2D(L, i, j).val[0]) - cvGet2D(T, i, j).val[0] > 0) ? 1 : 0));
                }
        }

//    cvCopyImage(pY, pResult);
    for (int i = 0; i < Y->height; ++i)
        for (int j = 0; j < Y->width; ++j)
            cvSet2D(aResult, i, j, cvScalar(cvGet2D(Y, i, j).val[0]*255.0, cvGet2D(Y, i, j).val[1]*255.0, cvGet2D(Y, i, j).val[2]*255.0));

    cvReleaseImage(&imgGray);
    cvReleaseImage(&kL);
    cvReleaseImage(&kF);
    cvReleaseImage(&S);
    cvReleaseImage(&F); 
    cvReleaseImage(&L); 
    cvReleaseImage(&T); 
    cvReleaseImage(&Y); 
    }

} // namespace media
