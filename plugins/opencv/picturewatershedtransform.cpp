#include "picturewatershedtransform.h"

using namespace cv;
using namespace std;

namespace media {

PictureWatershedTransform::PictureWatershedTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    }

void PictureWatershedTransform::process()
    {
    vector<vector<Point> > contours;
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == PointsFrame::Dimensions) &&
                (frame->getDimensionT(PointsFrame::Axis).mResolution == PointsFrame::MaxAxis))
                {
                vector<Point> contour;
                contour.resize(frame->getDimensionT(PointsFrame::Index).mResolution);
                int point[PointsFrame::Dimensions] = {0, 0};
                for (point[PointsFrame::Index] = 0; point[PointsFrame::Index] < frame->getDimensionT(PointsFrame::Index).mResolution; ++point[PointsFrame::Index])
                    {
                    point[PointsFrame::Axis] = PointsFrame::XAxis;
                    contour[point[PointsFrame::Index]].x = frame->getSampleT(point);
                    point[PointsFrame::Axis] = PointsFrame::YAxis;
                    contour[point[PointsFrame::Index]].y = frame->getSampleT(point);
                    }
                contours.push_back(contour);
                }
            else if (frame->getMaxDimension() == IplImageFrame::Dimensions)
                {
                mSrcFrame.resizeAndCopyFrame(*frame);
                mPictureFrame.setSourceName(frame->getSourceName());
                }
            }

    IplImage *srcImg = (IplImage*)mSrcFrame;
    if (!srcImg || !contours.size())
        return;

    IplImage *markers = cvCreateImage(cvGetSize(srcImg), IPL_DEPTH_32S, 1);
    cvZero(markers);

    //draws contours on the markers image from CvSeq* contours
    for (size_t i = 0; i < contours.size(); ++i)
        drawContours(Mat(markers), contours, i, Scalar(i + 1, i + 1, i + 1), -1, 8, noArray(), -1);

    CvMat* color_tab = cvCreateMat(1, contours.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); ++i)
        {
        uchar* ptr = color_tab->data.ptr + i*3;
        ptr[0] = (uchar)(theRNG()(180) + 50);
        ptr[1] = (uchar)(theRNG()(180) + 50);
        ptr[2] = (uchar)(theRNG()(180) + 50);
        }

    //segment the image using markers
    cvWatershed(srcImg, markers);

    IplImage *wshed = cvCloneImage(srcImg);
    cvZero(wshed);

    // paint the watershed image
    for (int i = 0; i < markers->height; ++i)
        for (int j = 0; j < markers->width; ++j)
        {
        int idx = CV_IMAGE_ELEM(markers, int, i, j);
        uchar* dst = &CV_IMAGE_ELEM(wshed, uchar, i, j*3);
        if (idx == -1)
            dst[0] = dst[1] = dst[2] = (uchar)255;
        else if (idx <= 0 || idx > (int)contours.size())
            dst[0] = dst[1] = dst[2] = (uchar)0; // should not get here
        else
            {
            uchar* ptr = color_tab->data.ptr + (idx-1)*3;
            dst[0] = ptr[0]; dst[1] = ptr[1]; dst[2] = ptr[2];
            }
        }

//    IplImage *img_gray = cvCloneImage(srcImg);
//    IplImage *img_bw = cvCreateImage(cvGetSize(srcImg), 8, 1);
//    cvCvtColor(img, img_bw, CV_BGR2GRAY);
//    cvCvtColor(img_bw, img_gray, CV_GRAY2BGR);
//    cvAddWeighted(wshed, 0.5, img_gray, 0.5, 0, wshed);
    mPictureFrame.resizeAndCopyImage(*wshed);

    cvReleaseMat(&color_tab);
    cvReleaseImage(&wshed);
    cvReleaseImage(&markers);

    emit framesReady();
    }

} // namespace media
