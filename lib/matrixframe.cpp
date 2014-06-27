#include "matrixframe.h"
#include <QDebug>

using namespace media;

MatrixFrame::MatrixFrame() : FrameBase(Dimensions)
    {
    }

const MatrixFrame& MatrixFrame::operator=(const Matrix& aMatrix)
    {
    mMatrix = aMatrix;
    mDimensions[VectorNo].mResolution = mMatrix.size();
    mDimensions[ParamNo].mResolution = mMatrix[0].size();
    return *this;
    }

void MatrixFrame::resize(const int *aSize)
    {
    setMaxVectors(aSize[VectorNo]);
    setVectorSizeT(aSize[ParamNo]);
    }

void MatrixFrame::setMaxVectors(int aSize)
    {
    mDimensions[VectorNo].mResolution = aSize;
    if (mMatrix.size() < static_cast<Matrix::size_type>(aSize))
        mMatrix.resize(aSize);
    }

void MatrixFrame::setVectorSizeT(int aSize)
    {
    mDimensions[ParamNo].mResolution = aSize;
    Q_ASSERT(mMatrix.size() > 0);
    for (Matrix::size_type i = 0; i < mMatrix.size(); ++i)
        if (mMatrix[i].size() < static_cast<Vector::size_type>(aSize))
            mMatrix[i].resize(aSize);
    }

qreal MatrixFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(inRange(aPoint));
    return mMatrix.at(aPoint[VectorNo]).at(aPoint[ParamNo]);
    }

void MatrixFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(inRange(aPoint));
    mMatrix[ aPoint[VectorNo] ][ aPoint[ParamNo] ] = aValue;
    }

void MatrixFrame::appendVectorT(const Vector& aVector)
    {
    Q_ASSERT(isVectorCompatible(aVector));
    ++mDimensions[VectorNo].mResolution;
    mDimensions[ParamNo].mResolution = aVector.size();
    mMatrix.push_back(aVector);
    }

void MatrixFrame::setVectorT(const Vector& aVector, int aVectorNo)
    {
    Q_ASSERT(aVectorNo < mMatrix.size());
    Q_ASSERT(isVectorCompatible(aVector));
    mMatrix[aVectorNo] = aVector;
    }

bool MatrixFrame::setVector(const Vector& aVector, int aVectorNo)
    {
    bool ret = ((aVectorNo < mDimensions[VectorNo].mResolution) && (isVectorCompatible(aVector)));
    if (ret)
        mMatrix[aVectorNo] = aVector;
    return ret;
    }

bool MatrixFrame::inRange(const int *aPoint) const
    {
    return (static_cast<Matrix::size_type>(aPoint[VectorNo]) < mMatrix.size())
        && (static_cast<Vector::size_type>(aPoint[ParamNo]) < mMatrix.at(aPoint[VectorNo]).size());
    }

bool MatrixFrame::isVectorCompatible(const Vector& aVector) const
    {
    bool ret = true;
    for (int i = 0; i < mMatrix.size(); ++i)
        if (mMatrix.at(i).size() != aVector.size())
            ret = false;
    return ret;
    }
