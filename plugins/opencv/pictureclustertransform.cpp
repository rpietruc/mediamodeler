#include "pictureclustertransform.h"
#include "pictureframes.h"

namespace media {

PictureClusterTransform::PictureClusterTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("clustersNo", 10);
    }

void PictureClusterTransform::process()
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
                mPictureFrame.resize(srcFrame.getDimensionT(PictureRGBFrame::Width).mResolution,
                                     srcFrame.getDimensionT(PictureRGBFrame::Height).mResolution);

                CvMat *pictureMat = createMatFromImage(srcFrame);
                CvMat *clustersMat = cvCreateMat(srcFrame.getDimensionT(PictureRGBFrame::Width).mResolution*srcFrame.getDimensionT(PictureRGBFrame::Height).mResolution, 1, CV_32SC1);

//                cvKMeansTest(pictureMat, clustersMat, mPictureFrame.getDimensionT(PictureRGBFrame::Channels).mResolution);
                cvKMeans2(pictureMat, property("clustersNo").toInt(), clustersMat, cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 50, 0.1));
                imageFromMat(mPictureFrame, clustersMat);

                cvReleaseMat(&clustersMat);
                cvReleaseMat(&pictureMat);

                emit framesReady();
                break;
                }
            }
    }

CvMat* PictureClusterTransform::createMatFromImage(const IplImage* img)
    {
    int dim = img->nChannels + 2;
    CvMat *mat = cvCreateMat(img->width*img->height, 1, CV_32FC(dim));
    int row = 0, col = 0, idx = 0;
    for (int h = 0; h < img->height; ++h)
        {
        col = row;
        for (int w = 0; w < img->width; ++w)
            {
            int k = 0;
            for (; k < img->nChannels; ++k, ++col, ++idx)
                {
                mat->data.fl[idx] = static_cast<unsigned char>(img->imageData[col]);
                mat->data.fl[idx] /= 255.0;
                }
            mat->data.fl[idx++] = static_cast<float>(w)/img->width/10;
            mat->data.fl[idx++] = static_cast<float>(h)/img->height/10;
            }
        row += img->widthStep;
        }
    return mat;
    }

void PictureClusterTransform::imageFromMat(IplImage* img, const CvMat *mtx)
    {
    int row = 0, col = 0, idx = 0;
    for (int h = 0; h < img->height; ++h)
        {
        col = row;
        for (int w = 0; w < img->width; ++w, ++col, ++idx)
            img->imageData[col] = static_cast<unsigned>(mtx->data.i[idx])*31;
        row += img->widthStep;
        }
    }

void PictureClusterTransform::cvKMeansTest(const CvMat *pic, CvMat *cls, int dim)
    {
    int col = 0, idx = 0;
    for (int h = 0; h < pic->height; h++)
        for (int w = 0; w < pic->width; ++w, ++idx)
            {
            cls->data.i[idx] = 0;
            for (int k = 0; k < dim; ++k, ++col)
                cls->data.i[idx] += pic->data.fl[col];
            cls->data.i[idx] /= 31*dim;
            }
    }

} // namespace media

