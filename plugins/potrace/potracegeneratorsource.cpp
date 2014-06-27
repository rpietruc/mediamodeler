#include "potracegeneratorsource.h"
#include <math.h>

namespace media {

PotraceGeneratorSource::PotraceGeneratorSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("width", 800);
    setProperty("height", 600);
    mImageFrame.setSourceName(objectName());
    }

void PotraceGeneratorSource::process()
    {
    // Create an image
    if (!mImageFrame.isEmpty())
        emit processingCompleted();
    else
        {
        // create a bitmap
        mImageFrame.resize(property("width").toInt(), property("height").toInt());

        // fill the bitmap with some pattern
        for (int x = 0; x < mImageFrame.getDimensionT(PotraceImageFrame::Width).mResolution; ++x)
            for (int y = 0; y < mImageFrame.getDimensionT(PotraceImageFrame::Height).mResolution; ++y)
                {
                int point[PotraceImageFrame::Dimensions] = {x, y};
                bool func = ((x*x + y*y*y) % 10000 < 5000);
                mImageFrame.setSampleT(point, func ? 1 : 0);
                }
        emit framesReady();
        }
    }
} // namespace media
