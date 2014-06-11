#ifndef MEDIA_MATRIXFRAME_H
#define MEDIA_MATRIXFRAME_H

#include "framebase.h"

namespace media {

typedef QVector<qreal> Vector;
typedef QVector<Vector> Matrix;

class MEDIAMODELSHARED_EXPORT MatrixFrame : public FrameBase
    {
public:
    enum { VectorNo = 0, ParamNo, Dimensions };
    explicit MatrixFrame() : FrameBase(Dimensions) {}

    const MatrixFrame& operator=(const Matrix& aMatrix)
        {
        mMatrix = aMatrix;
        mDimensions[ParamNo].mResolution = mMatrix.size();
        mDimensions[VectorNo].mResolution = mMatrix[0].size();
        return *this;
        }

    void setMaxVectors(int aSize)
        {
        mDimensions[VectorNo].mResolution = aSize;
        if (mMatrix.size() < static_cast<Matrix::size_type>(aSize))
            mMatrix.resize(aSize);
        }

    void setVectorSizeT(int aSize)
        {
        mDimensions[ParamNo].mResolution = aSize;
        Exc::throwExcCodeIfFalse(mMatrix.size() > 0, 0, __FILE__, __LINE__);
        for (Matrix::size_type i = 0; i < mMatrix.size(); ++i)
            if (mMatrix[i].size() < static_cast<Vector::size_type>(aSize))
                mMatrix[i].resize(aSize);
        }

    qreal getSampleT(const int *aPoint) const
        {
        Exc::throwExcCodeIfFalse(inRange(aPoint), 0, __FILE__, __LINE__);
        return mMatrix.at(aPoint[VectorNo]).at(aPoint[ParamNo]);
        }

    void setSampleT(const int *aPoint, qreal aValue)
        {
        Exc::throwExcCodeIfFalse(inRange(aPoint), 0, __FILE__, __LINE__);
        mMatrix[ aPoint[VectorNo] ][ aPoint[ParamNo] ] = aValue;
        }

    void appendVectorT(const Vector& aVector)
        {
        Exc::throwExcCodeIfFalse(isVectorCompatible(aVector), 0, __FILE__, __LINE__);
        ++mDimensions[VectorNo].mResolution;
        mDimensions[ParamNo].mResolution = aVector.size();
        mMatrix.push_back(aVector);
        }

    void setVectorT(const Vector& aVector, int aVectorNo)
        {
        Exc::throwExcCodeIfFalse(aVectorNo < mMatrix.size(), 0, __FILE__, __LINE__);
        Exc::throwExcCodeIfFalse(isVectorCompatible(aVector), 0, __FILE__, __LINE__);
        mMatrix[aVectorNo] = aVector;
        }

    bool setVector(const Vector& aVector, int aVectorNo)
        {
        bool ret = ((aVectorNo < mDimensions[VectorNo].mResolution) && (isVectorCompatible(aVector)));
        if (ret)
            mMatrix[aVectorNo] = aVector;
        return ret;
        }

    bool inRange(const int *aPoint) const
        {
        return (static_cast<Matrix::size_type>(aPoint[VectorNo]) < mMatrix.size())
            && (static_cast<Vector::size_type>(aPoint[ParamNo]) < mMatrix.at(aPoint[VectorNo]).size());
        }

protected:
    bool isVectorCompatible(const Vector& aVector) const
        {
        bool ret = true;
        for (int i = 0; i < mMatrix.size(); ++i)
            if (mMatrix.at(i).size() != aVector.size())
                ret = false;
        return ret;
        }

private:
    Matrix mMatrix;
    };

} // namespace media

#endif // MEDIA_MATRIXFRAME_H
