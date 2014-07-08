#include "matrixfiledestination.h"
#include "../armadillo/armadilloframes.h"
#include <mlpack/core.hpp>

using namespace mlpack;

namespace media {

MatrixFileDestination::MatrixFileDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("fileName", "output.csv");
    }

void MatrixFileDestination::process()
    {
    MatrixFrame matrixFrame;

    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (matrixFrame.isCopyable(*frame))
                {
                matrixFrame.resizeAndCopyFrame(*frame);
                data::Save(property("fileName").toString().toStdString(), (arma::mat&)matrixFrame, true);
                emit framesProcessed();
                }
            }
    }

} // namespace media
