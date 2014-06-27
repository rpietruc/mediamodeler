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

    explicit MatrixFrame();
    const MatrixFrame& operator=(const Matrix& aMatrix);
    virtual void resize(const int *aSize);
    void setMaxVectors(int aSize);
    void setVectorSizeT(int aSize);
    virtual qreal getSampleT(const int *aPoint) const;
    virtual void setSampleT(const int *aPoint, qreal aValue);
    void appendVectorT(const Vector& aVector);
    void setVectorT(const Vector& aVector, int aVectorNo);
    bool setVector(const Vector& aVector, int aVectorNo);
    bool inRange(const int *aPoint) const;

protected:
    bool isVectorCompatible(const Vector& aVector) const;

private:
    Matrix mMatrix;
    };

} // namespace media

#endif // MEDIA_MATRIXFRAME_H
