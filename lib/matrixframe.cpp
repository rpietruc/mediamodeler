#include "matrixframe.h"
#include <string.h>

using namespace media;

double MatrixFrame::getSampleT(const int *aPoint) const
    {
    Exc::throwExcCodeIfFalse(static_cast<Matrix::size_type>(aPoint[VectorNo]) < mMatrix.size(), 0, __FILE__, __LINE__);
    Exc::throwExcCodeIfFalse(static_cast<Vector::size_type>(aPoint[SampleNo]) < mMatrix.at(aPoint[VectorNo]).size(), 0, __FILE__, __LINE__);
    return mMatrix.at(aPoint[VectorNo]).at(aPoint[SampleNo]);
    }

void MatrixFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Exc::throwExcCodeIfFalse(static_cast<Matrix::size_type>(aPoint[VectorNo]) < mMatrix.size(), 0, __FILE__, __LINE__);
    Exc::throwExcCodeIfFalse(static_cast<Vector::size_type>(aPoint[SampleNo]) < mMatrix.at(aPoint[VectorNo]).size(), 0, __FILE__, __LINE__);
    mMatrix[ aPoint[VectorNo] ][ aPoint[SampleNo] ] = aValue;
    }

void MatrixFrame::setMaxSamplesT(int aSize)
    {
    mDimensions[SampleNo].mResolution = aSize;
    Exc::throwExcCodeIfFalse(mMatrix.size() > 0, 0, __FILE__, __LINE__);
    for (Matrix::size_type i = 0; i < mMatrix.size(); ++i)
        if (mMatrix[i].size() < static_cast<Vector::size_type>(aSize))
            mMatrix[i].resize(aSize);
    }

void MatrixFrame::setMaxVectorsT(int aSize)
    {
    mDimensions[VectorNo].mResolution = aSize;
    if (mMatrix.size() < static_cast<Matrix::size_type>(aSize))
        mMatrix.resize(aSize);
    }

double Matrix3DFrame::getSampleT(const int *aPoint) const
    {
    Exc::throwExcCodeIfFalse((static_cast<Matrix3D::size_type>(aPoint[PlaneNo]) < mMatrix3D.size()), 0, __FILE__, __LINE__);
    Exc::throwExcCodeIfFalse((static_cast<Matrix::size_type>(aPoint[VectorNo]) < mMatrix3D.at(aPoint[PlaneNo]).size()), 0, __FILE__, __LINE__);
    Exc::throwExcCodeIfFalse((static_cast<Vector::size_type>(aPoint[SampleNo]) < mMatrix3D.at(aPoint[PlaneNo]).at(aPoint[VectorNo]).size()), 0, __FILE__, __LINE__);
    return mMatrix3D.at(aPoint[PlaneNo]).at(aPoint[VectorNo]).at(aPoint[SampleNo]);
    }

void Matrix3DFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Exc::throwExcCodeIfFalse((static_cast<Matrix3D::size_type>(aPoint[PlaneNo]) < mMatrix3D.size()), 0, __FILE__, __LINE__);
    Exc::throwExcCodeIfFalse((static_cast<Matrix::size_type>(aPoint[VectorNo]) < mMatrix3D.at(aPoint[PlaneNo]).size()), 0, __FILE__, __LINE__);
    Exc::throwExcCodeIfFalse((static_cast<Vector::size_type>(aPoint[SampleNo]) < mMatrix3D.at(aPoint[PlaneNo]).at(aPoint[VectorNo]).size()), 0, __FILE__, __LINE__);
    mMatrix3D[ aPoint[PlaneNo] ][ aPoint[VectorNo] ][ aPoint[SampleNo] ] = aValue;
    }

void Matrix3DFrame::setMaxSamplesT(int aSize)
    {
    mDimensions[SampleNo].mResolution = aSize;
    Exc::throwExcCodeIfFalse((mMatrix3D.size() > 0), 0, __FILE__, __LINE__);
    for (Matrix3D::size_type i = 0; i < mMatrix3D.size(); ++i)
        {
        Exc::throwExcCodeIfFalse((mMatrix3D[i].size() > 0), 0, __FILE__, __LINE__);
        for (Matrix::size_type j = 0; j < mMatrix3D[i].size(); ++j)
            if (mMatrix3D[i][j].size()  < static_cast<Vector::size_type>(aSize))
                mMatrix3D[i][j].resize(aSize);
        }
    }

void Matrix3DFrame::setMaxVectorsT(int aSize)
    {
    mDimensions[VectorNo].mResolution = aSize;
    Exc::throwExcCodeIfFalse((mMatrix3D.size() > 0), 0, __FILE__, __LINE__);
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
