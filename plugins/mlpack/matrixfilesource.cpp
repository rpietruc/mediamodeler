#include "matrixfilesource.h"
#include "../armadillo/armadilloframes.h"

#include <mlpack/core.hpp>

using namespace mlpack;

namespace media {

MatrixFileSource::MatrixFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mMatrixFrame(new MatrixFrame)
    {
    setProperty("fileName", "input.csv");
    }

void MatrixFileSource::process()
    {
    if (data::Load(property("fileName").toString().toStdString(), (arma::mat&)*mMatrixFrame, true))
        emit framesReady();
    }

} // namespace media
