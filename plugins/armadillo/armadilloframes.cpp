#include "armadilloframes.h"
#include "exceptions.h"

using namespace arma;

namespace media {

ArrayFrame::ArrayFrame() :
    FrameBase(Dimensions)
    {
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

ArrayFrame::~ArrayFrame()
    {
    }

qreal ArrayFrame::getSampleT(const int *aPoint) const
    {
    return mMatrix(aPoint[Width], aPoint[Height]);
    }

void ArrayFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    mMatrix(aPoint[Width], aPoint[Height]) = aValue;
    }

void ArrayFrame::resize(int aWidth, int aHeight)
    {
    mMatrix.resize(aWidth, aHeight);
    mDimensions[Width].mResolution = aWidth;
    mDimensions[Height].mResolution = aHeight;
    }

void ArrayFrame::clear()
    {
    mMatrix.zeros();
    }

} // namespace media
