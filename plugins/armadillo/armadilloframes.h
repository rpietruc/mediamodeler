#ifndef MLPACKFRAMES_H
#define MLPACKFRAMES_H

#include "framebase.h"
#include <mlpack/core.hpp>
#include <armadillo>

namespace media {

class ArrayFrame : public FrameBase
    {
public:
    enum { Width, Height, Dimensions };

    explicit ArrayFrame();
    virtual ~ArrayFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    operator arma::mat&()  { return mMatrix; }
    const ArrayFrame& operator=(const arma::mat& aMatrix)
        {
        mMatrix = aMatrix;
        mDimensions[Width].mResolution = aMatrix.n_rows;
        mDimensions[Height].mResolution = aMatrix.n_cols;
        return *this;
        }

    void resize(const int* aSize) { resize(aSize[Width], aSize[Height]); }
    void resize(int aWidth, int aHeight);
    void clear();

private:
    arma::mat mMatrix;
    };

} // namespace media

#endif // MLPACKFRAMES_H
