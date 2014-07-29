#include "otbframes.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

using namespace itk;

namespace media {

VectorOtbFrame::VectorOtbFrame() :
    MultiChannelFrame(Dimensions),
    mImage(ImageType::New())
    {
    mDimensions[Channels].mResolution = 4;
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = mDimensions[Channels].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

VectorOtbFrame::~VectorOtbFrame()
    {
//    mImage->Delete();
    }

qreal VectorOtbFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    ImageType::IndexType index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    return mImage->GetPixel(index)[aPoint[Channels]];
    }

void VectorOtbFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    ImageType::IndexType index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    mImage->GetPixel(index)[aPoint[Channels]] = aValue;
    }

void VectorOtbFrame::resize(int aWidth, int aHeight)
    {
    ImageType::SizeType size;
    size[Width] = aWidth;
    size[Height] = aHeight;
    resize(size);
    }

void VectorOtbFrame::resize(const ImageType::SizeType& aSize)
    {
    if ((mDimensions[Width].mResolution != (int)aSize[Width]) || (mDimensions[Height].mResolution != (int)aSize[Height]))
        {
        mDimensions[Width].mResolution = aSize[Width];
        mDimensions[Height].mResolution = aSize[Height];

        ImageType::IndexType start;
        start.Fill(0);
        ImageType::RegionType region(start, aSize);
        mImage->SetRegions(region);
        mImage->SetVectorLength(getDimensionT(Channels).mResolution);
        mImage->Allocate();
        }
    }

void VectorOtbFrame::release()
    {
    resize(0, 0);
    }

void VectorOtbFrame::clear()
    {
    ImageType::PixelType pixel;
    pixel.SetSize(mDimensions.at(Channels).mResolution);
    for (int i = 0; i < mDimensions.at(Channels).mResolution; ++i)
        pixel.SetElement(i, 0);
    mImage->FillBuffer(pixel);
    }

const VectorOtbFrame& VectorOtbFrame::operator=(const ImageType::Pointer aImage)
    {
    resize((int)aImage->GetLargestPossibleRegion().GetSize()[0], (int)aImage->GetLargestPossibleRegion().GetSize()[1]);

    ImageType::IndexType index;
    for (index[Width] = 0; index[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]; ++index[Width])
        for (index[Height] = 0; index[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]; ++index[Height])
            mImage->SetPixel(index, aImage->GetPixel(index));
    }

ImageOtbFrame::ImageOtbFrame() :
    MonoChannelFrame(Dimensions),
    mImage(ImageType::New())
    {
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

void ImageOtbFrame::clear()
    {
    PixelType pixel = 0;
    mImage->FillBuffer(pixel);
    }

qreal ImageOtbFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    ImageType::IndexType index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    return mImage->GetPixel(index);
    }

void ImageOtbFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    ImageType::IndexType index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    mImage->GetPixel(index) = aValue;
    }

void ImageOtbFrame::resize(int aWidth, int aHeight)
    {
    ImageType::SizeType size;
    size[Width] = aWidth;
    size[Height] = aHeight;
    resize(size);
    }

void ImageOtbFrame::resize(const ImageType::SizeType& aSize)
    {
    if ((mDimensions[Width].mResolution != (int)aSize[Width]) || (mDimensions[Height].mResolution != (int)aSize[Height]))
        {
        mDimensions[Width].mResolution = aSize[Width];
        mDimensions[Height].mResolution = aSize[Height];

        ImageType::IndexType index;
        index.Fill(0);
        ImageType::RegionType region(index, aSize);
        mImage->SetRegions(region);
        mImage->Allocate();
        }
    }

void ImageOtbFrame::resizeAndCopyImage(const ImageType::Pointer aImage)
    {
    resize((int)aImage->GetLargestPossibleRegion().GetSize()[0], (int)aImage->GetLargestPossibleRegion().GetSize()[1]);

    ImageType::IndexType index;
    for (index[Width] = 0; index[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]; ++index[Width])
        for (index[Height] = 0; index[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]; ++index[Height])
            mImage->SetPixel(index, aImage->GetPixel(index));
    }

FloatOtbFrame::FloatOtbFrame() :
    MonoChannelFrame(Dimensions),
    mImage(ImageType::New())
    {
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

void FloatOtbFrame::clear()
    {
    PixelType pixel = 0;
    mImage->FillBuffer(pixel);
    }

qreal FloatOtbFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    ImageType::IndexType index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    return mImage->GetPixel(index);
    }

void FloatOtbFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    ImageType::IndexType index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    mImage->GetPixel(index) = aValue;
    }

void FloatOtbFrame::resize(int aWidth, int aHeight)
    {
    ImageType::SizeType size;
    size[Width] = aWidth;
    size[Height] = aHeight;
    resize(size);
    }

void FloatOtbFrame::resize(const ImageType::SizeType& aSize)
    {
    if ((mDimensions[Width].mResolution != (int)aSize[Width]) || (mDimensions[Height].mResolution != (int)aSize[Height]))
        {
        mDimensions[Width].mResolution = aSize[Width];
        mDimensions[Height].mResolution = aSize[Height];

        ImageType::IndexType index;
        index.Fill(0);
        ImageType::RegionType region(index, aSize);
        mImage->SetRegions(region);
        mImage->Allocate();
        }
    }

void FloatOtbFrame::resizeAndCopyImage(const ImageType::Pointer aImage)
    {
    resize((int)aImage->GetLargestPossibleRegion().GetSize()[0], (int)aImage->GetLargestPossibleRegion().GetSize()[1]);

    ImageType::IndexType index;
    for (index[Width] = 0; index[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]; ++index[Width])
        for (index[Height] = 0; index[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]; ++index[Height])
            mImage->SetPixel(index, aImage->GetPixel(index));
    }

} // namespace media

