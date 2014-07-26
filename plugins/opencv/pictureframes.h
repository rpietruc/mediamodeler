#ifndef PICTUREFRAMES_H
#define PICTUREFRAMES_H

#include "framebase.h"
#include <QString>
#include <vector>
#include <opencv/highgui.h>
#include <opencv/cv.h>

namespace media {

template <int ChannelsNo>
class IplImageFrame : public MultiChannelFrame
    {
public:
    enum { Width, Height, Channels, Dimensions };

    explicit IplImageFrame(int aDepth = IPL_DEPTH_8U) :
        MultiChannelFrame(Dimensions),
        mDepth(aDepth),
        mIplImage(NULL)
        {
        mDimensions[Channels].mResolution = ChannelsNo;
        mDimensions[Width].mDelta = mDimensions[Height].mDelta = mDimensions[Channels].mDelta = 1;
        release();
        }

    virtual ~IplImageFrame()
        {
        release();
        }

    qreal getSampleT(const int *aPoint) const
        {
        Q_ASSERT(mIplImage != NULL);
        Q_ASSERT(aPoint[Width] < mIplImage->width);
        Q_ASSERT(aPoint[Height] < mIplImage->height);
        Q_ASSERT(aPoint[Channels] < mIplImage->nChannels);
        Q_ASSERT((mIplImage->imageSize > aPoint[Height]*mIplImage->widthStep + aPoint[Width]*mIplImage->nChannels + aPoint[Channels]));
        return static_cast<quint8>(mIplImage->imageData[aPoint[Height]*mIplImage->widthStep + aPoint[Width]*mIplImage->nChannels + aPoint[Channels]]);
        }

    void setSampleT(const int *aPoint, qreal aValue)
        {
        Q_ASSERT(mIplImage != NULL);
        Q_ASSERT(aPoint[Width] < mIplImage->width);
        Q_ASSERT(aPoint[Height] < mIplImage->height);
        Q_ASSERT(aPoint[Channels] < mIplImage->nChannels);
        Q_ASSERT((mIplImage->imageSize > aPoint[Height]*mIplImage->widthStep + aPoint[Width]*mIplImage->nChannels + aPoint[Channels]));
        mIplImage->imageData[aPoint[Height]*mIplImage->widthStep + aPoint[Width]*mIplImage->nChannels + aPoint[Channels]] = aValue;
        }


    operator IplImage*() const { return mIplImage; }

    void resize(const int *aSize) { resize(aSize[Width], aSize[Height]); }
    void resize(int aWidth, int aHeight)
        {
        if (mIplImage && ((mDimensions[Width].mResolution != aWidth) || (mDimensions[Height].mResolution != aHeight)))
            release();
        if (!mIplImage)
            {
            mIplImage = cvCreateImage(cvSize(aWidth, aHeight), mDepth, mDimensions[Channels].mResolution);
            mDimensions[Width].mResolution = aWidth;
            mDimensions[Height].mResolution = aHeight;
            }
        }

    bool resizeAndCopyFrame(const FrameBase& aFrame)
        {
        if (!MultiChannelFrame::isCopyable(aFrame))
            return false;

        if (aFrame.getMaxDimension() < getMaxDimension())
            return MultiChannelFrame::resizeAndCopyFrame(aFrame);
        //else
        if (aFrame.getDimensionT(Channels).mResolution == 3)
            {
            IplImageFrame<3> frame;
            frame.MultiChannelFrame::resizeAndCopyFrame(aFrame);
            this->operator =(*(IplImage*)frame);
            }
        else if (aFrame.getDimensionT(Channels).mResolution == 1)
            {
            IplImageFrame<1> frame;
            frame.MultiChannelFrame::resizeAndCopyFrame(aFrame);
            this->operator =(*(IplImage*)frame);
            }
        else
            {
            Q_ASSERT(0 && "channels no not supported");
            return false;
            }
        return true;
        }

    void release()
        {
        if (mIplImage)
            {
            cvReleaseImage(&mIplImage);
            mIplImage = NULL;
            }
        mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
        }

    void clear()
        {
        if (mIplImage)
            cvZero(mIplImage);
        }

    const IplImageFrame& operator=(const IplImage &aIplImage)
        {
        resize(aIplImage.width, aIplImage.height);
        if (aIplImage.nChannels > mDimensions[Channels].mResolution)
            cvCvtColor(&aIplImage, mIplImage, CV_RGB2GRAY);
        else if (aIplImage.nChannels < mDimensions[Channels].mResolution)
            cvCvtColor(&aIplImage, mIplImage, CV_GRAY2RGB);
        else
            cvCopy(&aIplImage, mIplImage);
        return *this;
        }

private:
    int mDepth;
    IplImage* mIplImage;
    };

typedef IplImageFrame<1> PictureGrayFrame;
typedef IplImageFrame<3> PictureRGBFrame;

//
// TODO: add binary points frame for contour approx. (e.g. approxPolyDP)
// TODO: add 2+5-dim points frame for ellipse approx.
//
class PointsFrame : public FrameBase
    {
public:
    enum { XAxis, YAxis, MaxAxis };
    enum { Index, Axis, Dimensions };
    explicit PointsFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    void resize(const int* aPoint)
        {
        mPoints.resize(aPoint[Index]);
        }

    const PointsFrame& operator=(const std::vector<cv::Point> aPoints)
        {
        mPoints = aPoints;
        mDimensions[Index].mResolution = mPoints.size();
        return *this;
        }

    bool isCopyable(const FrameBase& aFrame) const
        {
        return FrameBase::isCopyable(aFrame)
            && (aFrame.getDimensionT(Axis).mResolution == MaxAxis);
        }

private:
    bool readFromFile(const QString& aFileName);
    void writeToFile(const QString& aFileName) const;

    std::vector<cv::Point> mPoints;
    };

//class RegionFrame : public FrameBase
//    {
//public:
//    enum { Width, Height, Channels, Dimensions };
//    explicit RegionFrame(QObject *aParent = 0) :
//        FrameBase(aParent),
//        mPictureSize(cv::Size(1024,768))
//        {
//        }

//    inline bool isValid() const { return !mRectVector.empty(); }
//    virtual double getDuration() const { return 0; }

//private:
//    std::vector<cv::Rect> mRectVector;
//    cv::Size mPictureSize;
//    };

} // namespace media

#endif // PICTUREFRAMES_H
