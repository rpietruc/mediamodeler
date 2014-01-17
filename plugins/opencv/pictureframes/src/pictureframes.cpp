#include "pictureframes.h"
#include <opencv/cv.h>
#include <QDebug>
#include <QFile>
#include <QTextStream>

namespace media {

IplImageFrame::IplImageFrame(int aChannels, int aDepth) :
    FrameBase(Dimensions),
    mDepth(aDepth),
    mIplImage(NULL)
    {
    mDimensions[Channels].mResolution = aChannels;
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = mDimensions[Channels].mDelta = 1;
    release();
    }

IplImageFrame::~IplImageFrame()
    {
    release();
    }

double IplImageFrame::getSample(const int *aPoint) const
    {
    Q_ASSERT(mIplImage);
    Q_ASSERT(aPoint[Width] < mIplImage->width);
    Q_ASSERT(aPoint[Height] < mIplImage->height);
    Q_ASSERT(aPoint[Channels] < mIplImage->nChannels);

    Q_ASSERT(mIplImage->imageSize > aPoint[Height]*mIplImage->widthStep + aPoint[Width]*mIplImage->nChannels + aPoint[Channels]);
    return static_cast<quint8>(mIplImage->imageData[aPoint[Height]*mIplImage->widthStep + aPoint[Width]*mIplImage->nChannels + aPoint[Channels]]);
    }

void IplImageFrame::resize(int aWidth, int aHeight)
    {
    if (mIplImage && ((mDimensions[Width].mResolution != aWidth) || (mDimensions[Height].mResolution != aHeight)))
        release();

    if (!mIplImage)
        {
        mDimensions[Width].mResolution = aWidth;
        mDimensions[Height].mResolution = aHeight;
        mIplImage = cvCreateImage(cvSize(mDimensions[Width].mResolution, mDimensions[Height].mResolution), mDepth, mDimensions[Channels].mResolution);
        }
    }

void IplImageFrame::release()
    {
    if (mIplImage)
        {
        cvReleaseImage(&mIplImage);
        mIplImage = NULL;
        }
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

void IplImageFrame::clear()
    {
    if (mIplImage)
        cvZero(mIplImage);
    }

void IplImageFrame::resizeAndCopyImage(const IplImage& aIplImage)
    {
    resize(aIplImage.width, aIplImage.height);

    if (aIplImage.nChannels == mIplImage->nChannels)
        cvCopy(&aIplImage, mIplImage);
    else if (aIplImage.nChannels > mIplImage->nChannels)
        cvCvtColor(&aIplImage, mIplImage, CV_RGB2GRAY);
    else
        cvCvtColor(&aIplImage, mIplImage, CV_GRAY2RGB);
    }

void IplImageFrame::resizeAndCopyFrame(const FrameBase &aFrame)
    {
    if (((aFrame.getMaxDimension() == Dimensions) || (aFrame.getMaxDimension() == (Dimensions - 1))) && !aFrame.isEmpty())
        {
        resize(aFrame.getDimension(Width).mResolution, aFrame.getDimension(Height).mResolution);

        int point[Dimensions];
        int widthStep = 0;
        for (point[Height] = 0; point[Height] < mIplImage->height; ++point[Height], widthStep += mIplImage->widthStep)
            {
            int heightStep = 0;
            for (point[Width] = 0; point[Width] < mIplImage->width; ++point[Width], heightStep += mIplImage->nChannels)
                {
                double value = 0;
                for (point[Channels] = 0; point[Channels] < mIplImage->nChannels; ++point[Channels])
                    {
                    Q_ASSERT(mIplImage->imageSize > widthStep + heightStep + point[Channels]);
                    if ((aFrame.getMaxDimension() < Dimensions) || (aFrame.getDimension(Channels).mResolution > point[Channels]))
                        value = aFrame.getSample(point);
                    mIplImage->imageData[widthStep + heightStep + point[Channels]] = value;
                    }
                }
            }
        }
    }

void IplImageFrame::resizeAndCopy(const IplImageFrame &aPictureFrame)
    {
    IplImage* img = aPictureFrame;
    if (img)
        resizeAndCopyImage(*img);
    }

PointsFrame::PointsFrame() :
    FrameBase(Dimensions)
    {
    mDimensions[Axis].mDelta = 1;
    mDimensions[Axis].mResolution = MaxAxis;
    }

double PointsFrame::getSample(const int *aPoint) const
    {
    Q_ASSERT(aPoint[Axis] < MaxAxis);
    Q_ASSERT(aPoint[Index] < (int)mPoints.size());

    return (aPoint[Axis] == XAxis ? mPoints[aPoint[Index]].x : mPoints[aPoint[Index]].y);
    }

bool PointsFrame::readFromFile(const QString& aFileName)
    {
    QFile inFile(aFileName);
    if (inFile.open(QIODevice::Text|QIODevice::ReadOnly))
        {
        QTextStream inStream(&inFile);
        CvPoint point;
        while (!inStream.atEnd())
            {
            inStream >> point.x >> point.y;
            mPoints.push_back(point);
            }
        return true;
        }
    return false;
    }

void PointsFrame::writeToFile(const QString& aFileName) const
    {
    QFile outFile(aFileName);
    if (outFile.open(QIODevice::Text|QIODevice::ReadWrite))
        {
        QTextStream outStream(&outFile);
        foreach (CvPoint point, mPoints)
            outStream << point.x << point.y;
        }
    }

} // namespace media
