#ifndef MEDIA_QUBE_H
#define MEDIA_QUBE_H

#include "framebase.h"

namespace media {

typedef QVector<qreal> Vector;
typedef QVector<Vector> Matrix;
typedef QVector<Matrix> Qube;

class MEDIAMODELSHARED_EXPORT QubeFrame : public FrameBase
    {
public:
    enum { PlaneNo = 0, VectorNo, ParamNo, Dimensions };
    explicit QubeFrame() : FrameBase(Dimensions) {}

    void setMaxPlanes(int aSize)
        {
        mDimensions[PlaneNo].mResolution = aSize;
        if (mQube.size() < static_cast<Qube::size_type>(aSize))
            mQube.resize(aSize);
        }

    void setMaxVectorsT(int aSize)
        {
        mDimensions[VectorNo].mResolution = aSize;
        Exc::throwExcCodeIfFalse((mQube.size() > 0), 0, __FILE__, __LINE__);
        for (Qube::size_type i = 0; i < mQube.size(); ++i)
            {
            if (mQube[i].size() < static_cast<Matrix::size_type>(aSize))
                mQube[i].resize(aSize);
            }
        }

    void setVectorSizeT(int aSize)
        {
        mDimensions[ParamNo].mResolution = aSize;
        Exc::throwExcCodeIfFalse((mQube.size() > 0), 0, __FILE__, __LINE__);
        for (Qube::size_type i = 0; i < mQube.size(); ++i)
            {
            Exc::throwExcCodeIfFalse((mQube[i].size() > 0), 0, __FILE__, __LINE__);
            for (Matrix::size_type j = 0; j < mQube[i].size(); ++j)
                if (mQube[i][j].size()  < static_cast<Vector::size_type>(aSize))
                    mQube[i][j].resize(aSize);
            }
        }

    qreal getSampleT(const int *aPoint) const
        {
        Exc::throwExcCodeIfFalse(inRange(aPoint), 0, __FILE__, __LINE__);
        return mQube.at(aPoint[PlaneNo]).at(aPoint[ParamNo]).at(aPoint[VectorNo]);
        }

    void setSampleT(const int *aPoint, qreal aValue)
        {
        Exc::throwExcCodeIfFalse(inRange(aPoint), 0, __FILE__, __LINE__);
        mQube[ aPoint[PlaneNo] ][ aPoint[VectorNo] ][ aPoint[ParamNo] ] = aValue;
        }

    bool inRange(const int *aPoint) const
        {
        return (static_cast<Qube::size_type>(aPoint[PlaneNo]) < mQube.size())
            && (static_cast<Matrix::size_type>(aPoint[VectorNo]) < mQube.at(aPoint[PlaneNo]).size())
            && (static_cast<Vector::size_type>(aPoint[ParamNo]) < mQube.at(aPoint[PlaneNo]).at(aPoint[VectorNo]).size());
        }

protected:
    Qube mQube;
    };

} // namespace media

#endif // MEDIA_QUBE_H
