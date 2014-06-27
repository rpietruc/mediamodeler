#ifndef MEDIA_QUBE_H
#define MEDIA_QUBE_H

#include "matrixframe.h"

namespace media {

typedef QVector<Matrix> Qube;

class MEDIAMODELSHARED_EXPORT QubeFrame : public FrameBase
    {
public:
    enum { PlaneNo = 0, VectorNo, ParamNo, Dimensions };

    explicit QubeFrame();
    virtual void resize(const int *aSize);
    void setMaxPlanes(int aSize);
    void setMaxVectorsT(int aSize);
    void setVectorSizeT(int aSize);
    virtual qreal getSampleT(const int *aPoint) const;
    virtual void setSampleT(const int *aPoint, qreal aValue);
    bool inRange(const int *aPoint) const;

protected:
    Qube mQube;
    };

} // namespace media

#endif // MEDIA_QUBE_H
