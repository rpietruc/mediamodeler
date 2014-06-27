#ifndef PICTUREFRAMES_H
#define PICTUREFRAMES_H

#include "framebase.h"
#include <QString>
#include <vector>
#include <opencv/highgui.h>
#include <opencv/cv.h>

namespace media {

class IplImageFrame : public MultiChannelFrame
    {
public:
    enum { Width, Height, Channels, Dimensions };

    explicit IplImageFrame(int aChannels = 3, int aDepth = IPL_DEPTH_8U);
    virtual ~IplImageFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    operator IplImage*() const { return mIplImage; }

    void resize(const int *aSize) { resize(aSize[Width], aSize[Height], aSize[Channels]); }
    void resize(int aWidth, int aHeight, int aChannels);
    void release();
    void clear();

    const IplImageFrame& operator=(const IplImage &aIplImage);

private:
    int mDepth;
    IplImage* mIplImage;
    };

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
