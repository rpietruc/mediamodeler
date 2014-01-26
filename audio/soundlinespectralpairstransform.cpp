#include "soundlinespectralpairstransform.h"
#include <QDebug>
#include <QtPlugin>

#define FLOATING_POINT
#define VAR_ARRAYS
#include <lsp.c>

namespace media {

SoundLineSpectralPairsTransform::SoundLineSpectralPairsTransform(TransformFactory *aFactory, const QString &aObjectName) :
    TransformBase(aFactory, aObjectName)
{
}

bool SoundLineSpectralPairsTransform::process()
{
    const LinearPredictionCoefficientsFrame *lpcFrame = NULL;
    foreach (const FrameBase *frame, mCurrentFramesReadySet)
        if ((lpcFrame = qobject_cast<const LinearPredictionCoefficientsFrame*>(frame)) != NULL)
            break;

    Q_ASSERT(lpcFrame);
    if (lpcFrame->isValid())
    {
        if (!mLPCFrame.mCoefficients)
        {
            mLPCFrame.mOrder = lpcFrame->mOrder;
            mLPCFrame.mCoefficients = new float[mLPCFrame.mOrder];
        }
        memset(mLPCFrame.mCoefficients, 0, sizeof(*mLPCFrame.mCoefficients) *mLPCFrame.mOrder);
        if (lpc_to_lsp(lpcFrame->mCoefficients, lpcFrame->mOrder, mLPCFrame.mCoefficients, 4, 0.02, 0) > 0)
        {
            mLPCFrame.setTimeStamp(lpcFrame->getTimeStamp());
            mLPCFrame.setDuration(lpcFrame->getDuration());
            mLPCFrame.setSourceName(lpcFrame->getSourceName());

            return true;
        }
    }
    return false;
}

Q_EXPORT_PLUGIN2(PLUGINTARGETNAME, SoundLineSpectralPairsTransformFactory);

} // namespace media
