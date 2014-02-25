#ifndef IMAGEFRAMES_H
#define IMAGEFRAMES_H

#include "framebase.h"
#include <QString>
#include <itkImage.h>
#include <itkRGBPixel.h>

namespace media {

class ColorImageFrame : public FrameBase
    {
public:
    enum { Width, Height, Channels, Dimensions };

    typedef itk::RGBPixel<unsigned char> PixelType;
    typedef itk::Image<PixelType> ImageType;

    explicit ColorImageFrame();
    ~ColorImageFrame();

    qreal getSample(const int *aPoint) const;

    operator ImageType::Pointer() const { return mImage; }

    void resize(int aWidth, int aHeight);
    void resize(const ImageType::SizeType& aSize);
    void release();
    void clear();

    void resizeAndCopyFrame(const FrameBase &aFrame);
    void resizeAndCopyImage(const ImageType::Pointer aImage);
    void resizeAndCopy(const ColorImageFrame& aImageColorFrame);

private:
    ImageType::Pointer mImage;
    };

class GrayImageFrame : public FrameBase
    {
public:
    enum { Width, Height, Dimensions };

    typedef unsigned char PixelType;
    typedef itk::Image<PixelType> ImageType;

    explicit GrayImageFrame();

    qreal getSample(const int *aPoint) const;

    operator ImageType::Pointer() const { return mImage; }

    void resize(int aWidth, int aHeight);
    void clear();

    void resizeAndCopyFrame(const FrameBase &aFrame);
    void resizeAndCopyImage(const ImageType::Pointer aImage);
    void resizeAndCopy(const GrayImageFrame& aImageColorFrame);

private:
    ImageType::Pointer mImage;
    };

} // namespace media

#endif // IMAGEFRAMES_H
