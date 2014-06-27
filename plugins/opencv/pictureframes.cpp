#include "pictureframes.h"
#include <opencv/cv.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace media {

IplImageFrame::IplImageFrame(int aChannels, int aDepth) :
    MultiChannelFrame(Dimensions),
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

double IplImageFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(mIplImage != NULL);
    Q_ASSERT(aPoint[Width] < mIplImage->width);
    Q_ASSERT(aPoint[Height] < mIplImage->height);
    Q_ASSERT(aPoint[Channels] < mIplImage->nChannels);
    Q_ASSERT((mIplImage->imageSize > aPoint[Height]*mIplImage->widthStep + aPoint[Width]*mIplImage->nChannels + aPoint[Channels]));
    return static_cast<quint8>(mIplImage->imageData[aPoint[Height]*mIplImage->widthStep + aPoint[Width]*mIplImage->nChannels + aPoint[Channels]]);
    }

void IplImageFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(mIplImage != NULL);
    Q_ASSERT(aPoint[Width] < mIplImage->width);
    Q_ASSERT(aPoint[Height] < mIplImage->height);
    Q_ASSERT(aPoint[Channels] < mIplImage->nChannels);
    Q_ASSERT((mIplImage->imageSize > aPoint[Height]*mIplImage->widthStep + aPoint[Width]*mIplImage->nChannels + aPoint[Channels]));
    mIplImage->imageData[aPoint[Height]*mIplImage->widthStep + aPoint[Width]*mIplImage->nChannels + aPoint[Channels]] = aValue;
    }

void IplImageFrame::resize(int aWidth, int aHeight, int aChannels)
    {
    if (mIplImage && ((mDimensions[Width].mResolution != aWidth) || (mDimensions[Height].mResolution != aHeight) || (mDimensions[Channels].mResolution != aChannels)))
        release();

    if (!mIplImage)
        {
        mIplImage = cvCreateImage(cvSize(aWidth, aHeight), mDepth, aChannels);

        mDimensions[Width].mResolution = aWidth;
        mDimensions[Height].mResolution = aHeight;
        mDimensions[Channels].mResolution = aChannels;
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

const IplImageFrame& IplImageFrame::operator=(const IplImage& aIplImage)
    {
    resize(aIplImage.width, aIplImage.height, aIplImage.nChannels);
    cvCopy(&aIplImage, mIplImage);
    return *this;
    }

PointsFrame::PointsFrame() :
    FrameBase(Dimensions)
    {
    mDimensions[Axis].mDelta = 1;
    mDimensions[Axis].mResolution = MaxAxis;
    }

qreal PointsFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(aPoint[Axis] < MaxAxis);
    Q_ASSERT(aPoint[Index] < (int)mPoints.size());
    return (aPoint[Axis] == XAxis ? mPoints[aPoint[Index]].x : mPoints[aPoint[Index]].y);
    }

void PointsFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(aPoint[Axis] < MaxAxis);
    Q_ASSERT(aPoint[Index] < (int)mPoints.size());
    if (aPoint[Axis] == XAxis)
        mPoints[aPoint[Index]].x = aValue;
    else
        mPoints[aPoint[Index]].y = aValue;
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
