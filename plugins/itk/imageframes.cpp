#include "imageframes.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

using namespace itk;

namespace media {

ColorImageFrame::ColorImageFrame() :
    MultiChannelFrame(Dimensions),
    mImage(ImageType::New())
    {
    mDimensions[Channels].mResolution = PixelType::Length;
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = mDimensions[Channels].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

ColorImageFrame::~ColorImageFrame()
    {
//    mImage->Delete();
    }

qreal ColorImageFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);
    Q_ASSERT(aPoint[Channels] < PixelType::Length);

    Index<> index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    return mImage->GetPixel(index)[aPoint[Channels]];
    }

void ColorImageFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);
    Q_ASSERT(aPoint[Channels] < PixelType::Length);

    Index<> index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    mImage->GetPixel(index)[aPoint[Channels]] = aValue;
    }

void ColorImageFrame::resize(int aWidth, int aHeight)
    {
    Size<> size;
    size[Width] = aWidth;
    size[Height] = aHeight;
    resize(size);
    }

void ColorImageFrame::resize(const ImageType::SizeType& aSize)
    {
    if ((mDimensions[Width].mResolution != (int)aSize[Width]) || (mDimensions[Height].mResolution != (int)aSize[Height]))
        {
        mDimensions[Width].mResolution = aSize[Width];
        mDimensions[Height].mResolution = aSize[Height];

        Index<> index;
        index.Fill(0);
        ImageType::RegionType region(index, aSize);
        mImage->SetRegions(region);
        mImage->Allocate();
        }
    }

void ColorImageFrame::release()
    {
    resize(0, 0);
    }

void ColorImageFrame::clear()
    {
    PixelType pixel;
    pixel.Fill(0);
    mImage->FillBuffer(pixel);
    }

const ColorImageFrame& ColorImageFrame::operator=(const ImageType::Pointer aImage)
    {
    resize((int)aImage->GetLargestPossibleRegion().GetSize()[0], (int)aImage->GetLargestPossibleRegion().GetSize()[1]);

    Index<> index;
    for (index[Width] = 0; index[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]; ++index[Width])
        for (index[Height] = 0; index[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]; ++index[Height])
            mImage->SetPixel(index, aImage->GetPixel(index));

    return *this;
    }

GrayImageFrame::GrayImageFrame() :
    MonoChannelFrame(Dimensions),
    mImage(ImageType::New())
    {
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

void GrayImageFrame::clear()
    {
    mImage->FillBuffer(0);
    }

qreal GrayImageFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    Index<> index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    return mImage->GetPixel(index);
    }

void GrayImageFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    Index<> index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    mImage->GetPixel(index) = aValue;
    }

void GrayImageFrame::resize(int aWidth, int aHeight)
    {
    if ((mDimensions[Width].mResolution != aWidth) || (mDimensions[Height].mResolution != aHeight))
        {
        mDimensions[Width].mResolution = aWidth;
        mDimensions[Height].mResolution = aHeight;

        Index<> index;
        index.Fill(0);
        Size<> size;
        size[Width] = aWidth;
        size[Height] = aHeight;
        ImageType::RegionType region(index, size);
        mImage->SetRegions(region);
        mImage->Allocate();
        PixelType pixel = 0;
        mImage->FillBuffer(pixel);
        }
    }

void GrayImageFrame::resizeAndCopyImage(const ImageType::Pointer aImage)
    {
    resize((int)aImage->GetLargestPossibleRegion().GetSize()[0], (int)aImage->GetLargestPossibleRegion().GetSize()[1]);

    Index<> index;
    for (index[Width] = 0; index[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]; ++index[Width])
        for (index[Height] = 0; index[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]; ++index[Height])
            mImage->SetPixel(index, aImage->GetPixel(index));
    }

FloatImageFrame::FloatImageFrame() :
    MonoChannelFrame(Dimensions),
    mImage(ImageType::New())
    {
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

void FloatImageFrame::clear()
    {
    mImage->FillBuffer(0);
    }

qreal FloatImageFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    Index<> index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    return mImage->GetPixel(index);
    }

void FloatImageFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    Index<> index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    mImage->GetPixel(index) = aValue;
    }

void FloatImageFrame::resize(int aWidth, int aHeight)
    {
    if ((mDimensions[Width].mResolution != aWidth) || (mDimensions[Height].mResolution != aHeight))
        {
        mDimensions[Width].mResolution = aWidth;
        mDimensions[Height].mResolution = aHeight;

        Index<> index;
        index.Fill(0);
        Size<> size;
        size[Width] = aWidth;
        size[Height] = aHeight;
        ImageType::RegionType region(index, size);
        mImage->SetRegions(region);
        mImage->Allocate();
        PixelType pixel = 0;
        mImage->FillBuffer(pixel);
        }
    }

void FloatImageFrame::resizeAndCopyImage(const ImageType::Pointer aImage)
    {
    resize((int)aImage->GetLargestPossibleRegion().GetSize()[0], (int)aImage->GetLargestPossibleRegion().GetSize()[1]);

    Index<> index;
    for (index[Width] = 0; index[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]; ++index[Width])
        for (index[Height] = 0; index[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]; ++index[Height])
            mImage->SetPixel(index, aImage->GetPixel(index));
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

    return (aPoint[Axis] == XAxis ? mPoints[aPoint[Index]][0] : mPoints[aPoint[Index]][1]);
    }

void PointsFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(aPoint[Axis] < MaxAxis);
    Q_ASSERT(aPoint[Index] < (int)mPoints.size());

    if (aPoint[Axis] == XAxis)
        mPoints[aPoint[Index]][0] = aValue;
    else
        mPoints[aPoint[Index]][1] = aValue;
    }

} // namespace media

