#ifndef IMAGEFRAMES_H
#define IMAGEFRAMES_H

#include "framebase.h"
#include <QString>
#include <itkImage.h>
#include <itkRGBPixel.h>
#include <vector>

namespace media {

class ColorImageFrame : public MultiChannelFrame
    {
public:
    enum { Width, Height, Channels, Dimensions };

    typedef itk::RGBPixel<unsigned char> PixelType;
    typedef itk::Image<PixelType> ImageType;

    explicit ColorImageFrame();
    ~ColorImageFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    operator ImageType::Pointer() const { return mImage; }

    void resize(const int *aSize) { resize(aSize[Width], aSize[Height]); }
    void resize(int aWidth, int aHeight);
    void resize(const ImageType::SizeType& aSize);
    void release();
    void clear();

    const ColorImageFrame& operator=(const ImageType::Pointer aImage);

private:
    ImageType::Pointer mImage;
    };

class GrayImageFrame : public MonoChannelFrame
    {
public:
    enum { Width, Height, Dimensions };

    typedef unsigned char PixelType;
    typedef itk::Image<PixelType> ImageType;

    explicit GrayImageFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    operator ImageType::Pointer() const { return mImage; }

    void resize(const int *aSize) { resize(aSize[Width], aSize[Height]); }
    void resize(int aWidth, int aHeight);
    void clear();

    void resizeAndCopyImage(const ImageType::Pointer aImage);

private:
    ImageType::Pointer mImage;
    };

class FloatImageFrame : public MonoChannelFrame
    {
public:
    enum { Width, Height, Dimensions };

    typedef float PixelType;
    typedef itk::Image<PixelType> ImageType;

    explicit FloatImageFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    operator ImageType::Pointer() const { return mImage; }

    void resize(const int *aSize) { resize(aSize[Width], aSize[Height]); }
    void resize(int aWidth, int aHeight);
    void clear();

    void resizeAndCopyImage(const ImageType::Pointer aImage);

private:
    ImageType::Pointer mImage;
    };

class PointsFrame : public FrameBase
    {
public:
    enum { XAxis, YAxis, MaxAxis };
    enum { Index, Axis, Dimensions };
    explicit PointsFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    void resize(const int *aSize)
        {
        mPoints.resize(aSize[Index]);
        }

    void setPoints(const std::vector< itk::Point<int> > aPoints)
        {
        mPoints = aPoints;
        mDimensions[Index].mResolution = mPoints.size();
        }

private:
    std::vector< itk::Point<int> > mPoints;
    };

} // namespace media

#endif // IMAGEFRAMES_H
