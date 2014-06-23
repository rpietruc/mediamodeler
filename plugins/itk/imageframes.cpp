#include "imageframes.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

using namespace itk;

namespace media {

ColorImageFrame::ColorImageFrame() :
    FrameBase(Dimensions),
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

double ColorImageFrame::getSampleT(const int *aPoint) const
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

void ColorImageFrame::resizeAndCopyFrame(const FrameBase &aFrame)
    {
    if ((aFrame.getMaxDimension() == Dimensions) && !aFrame.isEmpty())
        {
        resize(aFrame.getDimensionT(Width).mResolution, aFrame.getDimensionT(Height).mResolution);

        int point[Dimensions];
        Index<> index;
        PixelType pixel;
        for (point[Height] = 0; point[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]; ++point[Height])
            {
            index[Height] = point[Height];
            for (point[Width] = 0; point[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]; ++point[Width])
                {
                index[Width] = point[Width];
                for (point[Channels] = 0; point[Channels] < PixelType::Length; ++point[Channels])
                    pixel[point[Channels]] = (aFrame.getDimensionT(Channels).mResolution > point[Channels] ? aFrame.getSampleT(point) : 0);
                mImage->SetPixel(index, pixel);
                }
            }
        }
    }

void ColorImageFrame::resizeAndCopyImage(const ImageType::Pointer aImage)
    {
    resize((int)aImage->GetLargestPossibleRegion().GetSize()[0], (int)aImage->GetLargestPossibleRegion().GetSize()[1]);

    Index<> index;
    for (index[Width] = 0; index[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]; ++index[Width])
        for (index[Height] = 0; index[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]; ++index[Height])
            mImage->SetPixel(index, aImage->GetPixel(index));
    }

void ColorImageFrame::resizeAndCopy(const ColorImageFrame& aImageColorFrame)
    {
    resizeAndCopyImage(aImageColorFrame);
    }

GrayImageFrame::GrayImageFrame() :
    FrameBase(Dimensions),
    mImage(ImageType::New())
    {
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

void GrayImageFrame::clear()
    {
    mImage->FillBuffer(0);
    }

double GrayImageFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    Index<> index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    return mImage->GetPixel(index);
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

void GrayImageFrame::resizeAndCopyFrame(const FrameBase &aFrame)
    {
    if (((aFrame.getMaxDimension() == Dimensions) || (aFrame.getMaxDimension() == ColorImageFrame::Dimensions)) && !aFrame.isEmpty())
        {
        resize(aFrame.getDimensionT(Width).mResolution, aFrame.getDimensionT(Height).mResolution);

        int point[ColorImageFrame::Dimensions];
        Index<> index;
        PixelType pixel;
        for (point[Height] = 0; point[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]; ++point[Height])
            {
            index[Height] = point[Height];
            for (point[Width] = 0; point[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]; ++point[Width])
                {
                index[Width] = point[Width];
                point[ColorImageFrame::Channels] = 0;
                pixel = aFrame.getSampleT(point);
                if ((aFrame.getMaxDimension() == ColorImageFrame::Dimensions) && (aFrame.getDimensionT(ColorImageFrame::Channels).mResolution > 1))
                    {
                    double sum = pixel;
                    for (point[ColorImageFrame::Channels] = 1; point[ColorImageFrame::Channels] < aFrame.getDimensionT(ColorImageFrame::Channels).mResolution; ++point[ColorImageFrame::Channels])
                        sum += aFrame.getSampleT(point);
                    pixel = sum / aFrame.getDimensionT(ColorImageFrame::Channels).mResolution;
                    }
                mImage->SetPixel(index, pixel);
                }
            }
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

void GrayImageFrame::resizeAndCopy(const GrayImageFrame& aImageColorFrame)
    {
    resizeAndCopyImage(aImageColorFrame);
    }

FloatImageFrame::FloatImageFrame() :
    FrameBase(Dimensions),
    mImage(ImageType::New())
    {
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

void FloatImageFrame::clear()
    {
    mImage->FillBuffer(0);
    }

double FloatImageFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(mImage.IsNotNull());

    Q_ASSERT(aPoint[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]);
    Q_ASSERT(aPoint[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]);

    Index<> index;
    index[Width] = aPoint[Width];
    index[Height] = aPoint[Height];
    return mImage->GetPixel(index);
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

void FloatImageFrame::resizeAndCopyFrame(const FrameBase &aFrame)
    {
    if (((aFrame.getMaxDimension() == Dimensions) || (aFrame.getMaxDimension() == ColorImageFrame::Dimensions)) && !aFrame.isEmpty())
        {
        resize(aFrame.getDimensionT(Width).mResolution, aFrame.getDimensionT(Height).mResolution);

        int point[ColorImageFrame::Dimensions];
        Index<> index;
        PixelType pixel;
        for (point[Height] = 0; point[Height] < (int)mImage->GetLargestPossibleRegion().GetSize()[Height]; ++point[Height])
            {
            index[Height] = point[Height];
            for (point[Width] = 0; point[Width] < (int)mImage->GetLargestPossibleRegion().GetSize()[Width]; ++point[Width])
                {
                index[Width] = point[Width];
                point[ColorImageFrame::Channels] = 0;
                pixel = aFrame.getSampleT(point);
                if ((aFrame.getMaxDimension() == ColorImageFrame::Dimensions) && (aFrame.getDimensionT(ColorImageFrame::Channels).mResolution > 1))
                    {
                    double sum = pixel;
                    for (point[ColorImageFrame::Channels] = 1; point[ColorImageFrame::Channels] < aFrame.getDimensionT(ColorImageFrame::Channels).mResolution; ++point[ColorImageFrame::Channels])
                        sum += aFrame.getSampleT(point);
                    pixel = sum / aFrame.getDimensionT(ColorImageFrame::Channels).mResolution;
                    }
                mImage->SetPixel(index, pixel);
                }
            }
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

void FloatImageFrame::resizeAndCopy(const FloatImageFrame& aImageColorFrame)
    {
    resizeAndCopyImage(aImageColorFrame);
    }

PointsFrame::PointsFrame() :
    FrameBase(Dimensions)
    {
    mDimensions[Axis].mDelta = 1;
    mDimensions[Axis].mResolution = MaxAxis;
    }

double PointsFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(aPoint[Axis] < MaxAxis);
    Q_ASSERT(aPoint[Index] < (int)mPoints.size());

    return (aPoint[Axis] == XAxis ? mPoints[aPoint[Index]][0] : mPoints[aPoint[Index]][1]);
    }

} // namespace media
