#include "armadilloframes.h"
#include "exceptions.h"

using namespace arma;

namespace media {

MatrixFrame::MatrixFrame() :
    FrameBase(Dimensions)
    {
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

MatrixFrame::~MatrixFrame()
    {
    }

qreal MatrixFrame::getSampleT(const int *aPoint) const
    {
    return 0;
    }

void MatrixFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    }

void MatrixFrame::resize(int aWidth, int aHeight)
    {
    mMatrix.resize(aWidth, aHeight);
    mDimensions[Width].mResolution = aWidth;
    mDimensions[Height].mResolution = aHeight;
    }

void MatrixFrame::clear()
    {
    }

} // namespace media
