#ifndef MEDIA_MATRIXFRAME_H
#define MEDIA_MATRIXFRAME_H

#include "framebase.h"

namespace media {

typedef QVector<qreal> Vector;
typedef QVector<Vector> Matrix;
typedef QVector<Matrix> Matrix3D;

class MEDIAMODELSHARED_EXPORT MatrixFrame : public FrameBase
    {
public:
    enum { SampleNo = 0, VectorNo, Dimensions };
    explicit MatrixFrame() : FrameBase(Dimensions) {}

    qreal getSampleT(const int *aPoint) const;

    void setMaxSamplesT(int aSize);
    void setMaxVectorsT(int aSize);

    void setSampleT(const int *aPoint, qreal aValue);

protected:
    Matrix mMatrix;
    };

class MEDIAMODELSHARED_EXPORT Matrix3DFrame : public FrameBase
    {
public:
    enum { SampleNo = 0, VectorNo, PlaneNo, Dimensions };
    explicit Matrix3DFrame() : FrameBase(Dimensions) {}

    qreal getSampleT(const int *aPoint) const;

    void setMaxVectorsT(int aSize);
    void setMaxSamplesT(int aSize);
    void setMaxPlanes(int aSize);

    void setSampleT(const int *aPoint, qreal aValue);

protected:
    Matrix3D mMatrix3D;
    };

} // namespace media

#endif // MEDIA_MATRIXFRAME_H
