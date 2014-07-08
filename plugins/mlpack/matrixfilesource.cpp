#include "matrixfilesource.h"

#include <mlpack/core.hpp>

using namespace mlpack;

namespace media {

MatrixFileSource::MatrixFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    FileListSource(aFactory, aObjectName)
    {
    }

void MatrixFileSource::process()
    {
    QString filename = getNextFilePath();
    if (filename.isEmpty())
        emit processingCompleted();

    else
        {
        arma::mat frame;
        if (data::Load(filename.toStdString(), frame, true))
            {
            mMatrixFrame = frame;
            emit framesReady();
            }
        }
    }

} // namespace media
