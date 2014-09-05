#include "picturebinarytransform.h"
#include "pictureframes.h"

namespace media {

void PCNNTransform(IplImage *pOriImg, IplImage *pResult);

PictureBinaryTransform::PictureBinaryTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
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
                PCNNTransform(srcFrame, mPictureFrame);
                emit framesReady();
                break;
                }
            }
    }

/**
 * source: http://www.oschina.net/code/piece_full?code=5828
 */
#define N 8

void PCNNTransform(IplImage *pOriImg, IplImage *pResult)
{
    int nRow = 0,nCol = 0;
    nRow = pOriImg->height;
    nCol = pOriImg->width;

    float k[3][3]={0.707,1,0.707,
        1, 1, 1,
        0.707,1,0.707};
 
    double dAF = 0.693;
    double dAL = 0.693;
    double dAE = 0.069;
    double dVF = 0.01;
    //double dVL = 1;
    double dVE = 0.2;
    double dB = 0.2;
 
    IplImage *pGray = cvCreateImage(cvGetSize(pOriImg),8,1);
    cvCvtColor(pOriImg,pGray,CV_BGR2GRAY);
    IplImage *pSUM1 = cvCreateImage(cvGetSize(pOriImg),8,1);
    CvMat km=cvMat(3,3,CV_32F,k);
 
    IplImage *pS = cvCreateImage(cvGetSize(pOriImg),8,1);
    IplImage *pS2 = cvCreateImage(cvGetSize(pOriImg),IPL_DEPTH_32F,1);
    IplImage *pF = cvCreateImage(cvGetSize(pOriImg),IPL_DEPTH_32F,1);
    IplImage *pL = cvCreateImage(cvGetSize(pOriImg),IPL_DEPTH_32F,1);
    IplImage *pE = cvCreateImage(cvGetSize(pOriImg),IPL_DEPTH_32F,1);
    IplImage *pY = cvCreateImage(cvGetSize(pOriImg),8,1);

    cvCopyImage(pGray,pS);
    for(int i = 0;i < nRow;i++)
    {
        for(int j = 0;j < nCol;j++)
        {
            CvScalar nPixel = cvGet2D(pS,i,j);
            CvScalar nNewPix;
            nNewPix.val[0] = nPixel.val[0] / 255.0;
            nNewPix.val[1] = nPixel.val[1] / 255.0;
            nNewPix.val[2] = nPixel.val[2] / 255.0;
            cvSet2D(pS2,i,j,nNewPix);
            //cout<<cvGet2D(pS2,i,j).val[0]<<"\t" ;
        }
        //cout<<endl;
    }

    for(int i = 0;i < nRow;i++)
    {
        for(int j = 0;j < nCol;j++)
        {
            CvScalar nNewPix;
            nNewPix.val[0] = 0;
            cvSet2D(pY,i,j,nNewPix);
        }
    }

    for(int i = 0;i < nRow;i++)
    {
        for(int j = 0;j < nCol;j++)
        {
            CvScalar nNewPix;
            nNewPix.val[0] = 2;
            cvSet2D(pE,i,j,nNewPix);
        }
    }

    //for(int i = 0;i < nRow;i++)
    //{
    //  for(int j = 0;j < nCol;j++)
    //  {
    //      cout<<cvGet2D(pGray,i,j).val[0]<<cvGet2D(pGray,i,j).val[1]<<cvGet2D(pGray,i,j).val[2]<<cvGet2D(pGray,i,j).val[3] ;
    //  }
    //}

    cvCopyImage(pS2,pF);
    cvCopyImage(pS2,pL);
 
    CvScalar cF,cL,cE;
    for(int n = 0;n < N;n++)
    {
        cvFilter2D(pY,pSUM1,&km);
        double dF,dL,dU,dE;
        for(int i = 0;i < nRow;i++)
        {
            for(int j = 0;j < nCol;j++)
            {
                dF = cvGet2D(pF,i,j).val[0];
                dF = exp(-dAF) * cvGet2D(pF,i,j).val[0] + cvGet2D(pS2,i,j).val[0] + dVF * cvGet2D(pSUM1,i,j).val[0];
                cF.val [0] = dF;
                cvSet2D(pF,i,j,cF);
                dL = cvGet2D(pL,i,j).val[0];
                dL = exp(-dAL) * dL + cvGet2D(pL,i,j).val[0] * cvGet2D(pSUM1,i,j).val[0];
                cL.val [0] = dL;
                cvSet2D(pL,i,j,cL);
                dU = cvGet2D(pF,i,j).val[0] * (1 + dB * cvGet2D(pL,i,j).val[0]);
                dE = cvGet2D(pE,i,j).val[0];
                dE = exp(-dAE) * cvGet2D(pE,i,j).val[0] + dVE * cvGet2D(pY,i,j).val[0];
                cE.val [0] = dE;
                cvSet2D(pE,i,j,cE);
                if(dU - dE > 0)
                {
                    CvScalar sca;
                    sca.val[0] = 255;
                    cvSet2D(pY,i,j,sca);
                }
                else
                {
                    CvScalar sca;
                    sca.val[0] = 0;
                    cvSet2D(pY,i,j,sca);
                }
                //cout<<cvGet2D(pF,i,j).val[0]<<"\t"<<cvGet2D(pL,i,j).val[0]<<"\t"<<cvGet2D(pE,i,j).val[0]<<endl;
            }
        }

        //Sleep(200);
    }
    cvCopyImage(pY, pResult);
}

} // namespace media
