#ifndef MLPACKFRAMES_H
#define MLPACKFRAMES_H

#include "framebase.h"
#include <armadillo>

namespace media {

class MatrixFrame : public FrameBase
    {
public:
    enum { Width, Height, Dimensions };

    explicit MatrixFrame();
    ~MatrixFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    operator arma::mat&()  { return mMatrix; }

    void resize(const int* aSize) { resize(aSize[Width], aSize[Height]); }
    void resize(int aWidth, int aHeight);
    void clear();

private:
    arma::mat mMatrix;
    };

} // namespace media

#endif // MLPACKFRAMES_H
