#ifndef PICTUREFRAMES_H
#define PICTUREFRAMES_H

#include "framebase.h"
#include <QString>
#include <vector>
#include <opencv/highgui.h>
#include <opencv/cv.h>

namespace media {

class IplImageFrame : public FrameBase
    {
public:
    enum { Width, Height, Channels, Dimensions };

    explicit IplImageFrame(int aChannels = 3, int aDepth = IPL_DEPTH_8U);
    virtual ~IplImageFrame();

    qreal getSample(const int *aPoint) const;

    operator IplImage*() const { return mIplImage; }

    void resize(int aWidth, int aHeight);
    void release();
    void clear();

    void resizeAndCopyImage(const IplImage &aIplImage);
    void resizeAndCopyFrame(const FrameBase &aFrame);
    void resizeAndCopy(const IplImageFrame &aPictureFrame);

private:
    int mDepth;
    IplImage* mIplImage;
    };

class PointsFrame : public FrameBase
    {
public:
    enum { XAxis, YAxis, MaxAxis };
    enum { Index, Axis, Dimensions };
    explicit PointsFrame();

    qreal getSample(const int *aPoint) const;
    const PointsFrame& operator=(const std::vector<cv::Point> aPoints)
        {
        mPoints = aPoints;
        mDimensions[Index].mResolution = mPoints.size();
        return *this;
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
