#include "matrixfiledestination.h"
#include "../armadillo/armadilloframes.h"
#include <mlpack/core.hpp>

using namespace mlpack;

namespace media {

MatrixFileDestination::MatrixFileDestination(ElementFactory *aFactory, const QString &aObjectName) :
    FileDestination(aFactory, aObjectName)
    {
    }

void MatrixFileDestination::process()
    {
    ArrayFrame matrixFrame;

    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (matrixFrame.isCopyable(*frame))
                {
                matrixFrame.resizeAndCopyFrame(*frame);
                data::Save(getNextFilePath().toStdString(), (arma::mat&)matrixFrame, true);
                emit framesProcessed();
                }
            }
    }

} // namespace media
