#include "qubeframe.h"
#include <QDebug>

using namespace media;

QubeFrame::QubeFrame() :
    FrameBase(Dimensions)
    {
    }

void QubeFrame::resize(const int *aSize)
    {
    setMaxPlanes(aSize[PlaneNo]);
    setMaxVectorsT(aSize[VectorNo]);
    setVectorSizeT(aSize[ParamNo]);
    }

void QubeFrame::setMaxPlanes(int aSize)
    {
    mDimensions[PlaneNo].mResolution = aSize;
    if (mQube.size() < static_cast<Qube::size_type>(aSize))
        mQube.resize(aSize);
    }

void QubeFrame::setMaxVectorsT(int aSize)
    {
    mDimensions[VectorNo].mResolution = aSize;
    Q_ASSERT(mQube.size() > 0);
    for (Qube::size_type i = 0; i < mQube.size(); ++i)
        {
        if (mQube[i].size() < static_cast<Matrix::size_type>(aSize))
            mQube[i].resize(aSize);
        }
    }

void QubeFrame::setVectorSizeT(int aSize)
    {
    mDimensions[ParamNo].mResolution = aSize;
    Q_ASSERT(mQube.size() > 0);
    for (Qube::size_type i = 0; i < mQube.size(); ++i)
        {
        Q_ASSERT(mQube[i].size() > 0);
        for (Matrix::size_type j = 0; j < mQube[i].size(); ++j)
            if (mQube[i][j].size()  < static_cast<Vector::size_type>(aSize))
                mQube[i][j].resize(aSize);
        }
    }

qreal QubeFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(inRange(aPoint));
    return mQube.at(aPoint[PlaneNo]).at(aPoint[VectorNo]).at(aPoint[ParamNo]);
    }

void QubeFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(inRange(aPoint));
    mQube[ aPoint[PlaneNo] ][ aPoint[VectorNo] ][ aPoint[ParamNo] ] = aValue;
    }

bool QubeFrame::inRange(const int *aPoint) const
    {
    return (static_cast<Qube::size_type>(aPoint[PlaneNo]) < mQube.size())
        && (static_cast<Matrix::size_type>(aPoint[VectorNo]) < mQube.at(aPoint[PlaneNo]).size())
        && (static_cast<Vector::size_type>(aPoint[ParamNo]) < mQube.at(aPoint[PlaneNo]).at(aPoint[VectorNo]).size());
    }
