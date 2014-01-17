#include "matrixframe.h"
#include <string.h>

using namespace media;

double MatrixFrame::getSample(const int *aPoint) const
    {
    Q_ASSERT(static_cast<Matrix::size_type>(aPoint[VectorNo]) < mMatrix.size());
    Q_ASSERT(static_cast<Vector::size_type>(aPoint[SampleNo]) < mMatrix.at(aPoint[VectorNo]).size());
    return mMatrix.at(aPoint[VectorNo]).at(aPoint[SampleNo]);
    }

void MatrixFrame::setSample(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(static_cast<Matrix::size_type>(aPoint[VectorNo]) < mMatrix.size());
    Q_ASSERT(static_cast<Vector::size_type>(aPoint[SampleNo]) < mMatrix.at(aPoint[VectorNo]).size());
    mMatrix[ aPoint[VectorNo] ][ aPoint[SampleNo] ] = aValue;
    }

void MatrixFrame::setMaxSamples(int aSize)
    {
    mDimensions[SampleNo].mResolution = aSize;
    Q_ASSERT(mMatrix.size() > 0);
    for (Matrix::size_type i = 0; i < mMatrix.size(); ++i)
        if (mMatrix[i].size() < static_cast<Vector::size_type>(aSize))
            mMatrix[i].resize(aSize);
    }

void MatrixFrame::setMaxVectors(int aSize)
    {
    mDimensions[VectorNo].mResolution = aSize;
    if (mMatrix.size() < static_cast<Matrix::size_type>(aSize))
        mMatrix.resize(aSize);
    }

double Matrix3DFrame::getSample(const int *aPoint) const
    {
    Q_ASSERT(static_cast<Matrix3D::size_type>(aPoint[PlaneNo]) < mMatrix3D.size());
    Q_ASSERT(static_cast<Matrix::size_type>(aPoint[VectorNo]) < mMatrix3D.at(aPoint[PlaneNo]).size());
    Q_ASSERT(static_cast<Vector::size_type>(aPoint[SampleNo]) < mMatrix3D.at(aPoint[PlaneNo]).at(aPoint[VectorNo]).size());
    return mMatrix3D.at(aPoint[PlaneNo]).at(aPoint[VectorNo]).at(aPoint[SampleNo]);
    }

void Matrix3DFrame::setSample(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(static_cast<Matrix3D::size_type>(aPoint[PlaneNo]) < mMatrix3D.size());
    Q_ASSERT(static_cast<Matrix::size_type>(aPoint[VectorNo]) < mMatrix3D.at(aPoint[PlaneNo]).size());
    Q_ASSERT(static_cast<Vector::size_type>(aPoint[SampleNo]) < mMatrix3D.at(aPoint[PlaneNo]).at(aPoint[VectorNo]).size());
    mMatrix3D[ aPoint[PlaneNo] ][ aPoint[VectorNo] ][ aPoint[SampleNo] ] = aValue;
    }

void Matrix3DFrame::setMaxSamples(int aSize)
    {
    mDimensions[SampleNo].mResolution = aSize;
    Q_ASSERT(mMatrix3D.size() > 0);
    for (Matrix3D::size_type i = 0; i < mMatrix3D.size(); ++i)
        {
        Q_ASSERT(mMatrix3D[i].size() > 0);
        for (Matrix::size_type j = 0; j < mMatrix3D[i].size(); ++j)
            if (mMatrix3D[i][j].size()  < static_cast<Vector::size_type>(aSize))
                mMatrix3D[i][j].resize(aSize);
        }
    }

void Matrix3DFrame::setMaxVectors(int aSize)
    {
    mDimensions[VectorNo].mResolution = aSize;
    Q_ASSERT(mMatrix3D.size() > 0);
    for (Matrix3D::size_type i = 0; i < mMatrix3D.size(); ++i)
        {
        if (mMatrix3D[i].size() < static_cast<Matrix::size_type>(aSize))
            mMatrix3D[i].resize(aSize);
        }
    }

void Matrix3DFrame::setMaxPlanes(int aSize)
    {
    mDimensions[PlaneNo].mResolution = aSize;
    if (mMatrix3D.size() < static_cast<Matrix3D::size_type>(aSize))
        mMatrix3D.resize(aSize);
    }
