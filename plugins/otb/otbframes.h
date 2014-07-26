#ifndef OTBFRAMES_H
#define OTBFRAMES_H

#include "framebase.h"
#include <QString>
#include <otbVectorImage.h>
#include <otbImage.h>
#include <vector>

namespace media {

class VectorOtbFrame : public MultiChannelFrame
    {
public:
    enum { Width, Height, Channels, Dimensions };

    static const unsigned int Dimension = 2;
    typedef double PixelType;
    typedef otb::VectorImage<PixelType, Dimension> ImageType;

    explicit VectorOtbFrame();
    ~VectorOtbFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    operator ImageType::Pointer() const { return mImage; }

    void resize(const int *aSize) { resize(aSize[Width], aSize[Height]); }
    void resize(int aWidth, int aHeight);
    void resize(const ImageType::SizeType& aSize);
    void release();
    void clear();

    const VectorOtbFrame& operator=(const ImageType::Pointer aImage);

private:
    ImageType::Pointer mImage;
    };

class ImageOtbFrame : public MonoChannelFrame
    {
public:
    enum { Width, Height, Dimensions };

    typedef unsigned char PixelType;
    static const unsigned int Dimension = 2;
    typedef otb::Image<PixelType, Dimension> ImageType;

    explicit ImageOtbFrame();

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

class FloatOtbFrame : public MonoChannelFrame
    {
public:
    enum { Width, Height, Dimensions };

    typedef float PixelType;
    static const unsigned int Dimension = 2;
    typedef otb::Image<PixelType, Dimension> ImageType;

    explicit FloatOtbFrame();

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

} // namespace media

#endif // OTBFRAMES_H
