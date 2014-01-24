#include "imagegeneratorsource.h"
#include <itkCurvatureFlowImageFilter.h>

using namespace itk;

namespace media {

ImageGeneratorSource::ImageGeneratorSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    mSize[0] = 800;
    mSize[1] = 600;
    mImageFrame.setSourceName(objectName());
    }

ElementBase::ParamList ImageGeneratorSource::getParams() const
    {
    ParamList ret;
    ret["Width"] = (int)mSize[0];
    ret["Height"] = (int)mSize[1];
    return ret;
    }

void ImageGeneratorSource::setParamValue(const QString& aName, const QVariant& aValue)
    {
    if (aName == "Width")
        {
        if (mSize[0] != aValue.toInt())
            {
            mImageFrame.release();
            mSize[0] = aValue.toInt();
            }
        }
    else if (aName == "Height")
        {
        if (mSize[1] != aValue.toInt())
            {
            mImageFrame.release();
            mSize[1] = aValue.toInt();
            }
        }
    }

void ImageGeneratorSource::process()
    {
    // Create an image
    if (!mImageFrame.isEmpty())
        emit processingCompleted();
    else
        {
        mImageFrame.resize(mSize);
        mImageFrame.clear();

        // Make a square
        ColorImageFrame::ImageType::IndexType pixelIndex;
        ColorImageFrame::PixelType pixel;
        pixel[0] = 255;
        pixel[1] = 0;
        pixel[2] = 0;
        for (pixelIndex[0] = 0; pixelIndex[0] < (int)mSize[0]; ++pixelIndex[0])
            for (pixelIndex[1] = 0; pixelIndex[1] < (int)mSize[1]; ++pixelIndex[1])
                if (drawEllipse((double)pixelIndex[0]/(double)mSize[0], (double)pixelIndex[1]/(double)mSize[1]))
                    ((ColorImageFrame::ImageType::Pointer)mImageFrame)->SetPixel(pixelIndex, pixel);

        emit framesReady();
        }
    }

bool ImageGeneratorSource::drawRect(double x, double y)
    {
    return (x > 0.25) && (x < 0.75) && (y > 0.25) && (y < 0.75);
    }

bool ImageGeneratorSource::drawEllipse(double x, double y)
    {
    return (x*x + y*y) < 0.75;
    }

} // namespace media
