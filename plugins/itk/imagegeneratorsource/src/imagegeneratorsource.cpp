#include "imagegeneratorsource.h"
#include <itkCurvatureFlowImageFilter.h>

using namespace itk;

namespace media {

ImageGeneratorSource::ImageGeneratorSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("width", 800);
    setProperty("height", 600);
    mImageFrame.setSourceName(objectName());
    }

void ImageGeneratorSource::process()
    {
    if ((int)mSize[0] != property("width").toInt())
        {
        mImageFrame.release();
        mSize[0] = property("width").toInt();
        }
    if ((int)mSize[1] != property("height").toInt())
        {
        mImageFrame.release();
        mSize[1] = property("height").toInt();
        }

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
