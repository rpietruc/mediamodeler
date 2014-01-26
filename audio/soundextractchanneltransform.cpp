#include "soundextractchanneltransform.h"
#include <QDebug>
#include <QtPlugin>

using namespace acoustics;

namespace media {

SoundExtractChannelTransform::SoundExtractChannelTransform(TransformFactory *aFactory, const QString &aObjectName) :
    TransformBase(aFactory, aObjectName),
    mChannelIndex(DefaultChannel)
{
    mChannelFrame.setChannelsNo(1);
}

void SoundExtractChannelTransform::showSettingsDialog()
{
    setChannelIndex(DefaultChannel);
}

void SoundExtractChannelTransform::initSettings(const QString& aSettings)
{
    setChannelIndex(aSettings.toInt());
}

void SoundExtractChannelTransform::setChannelIndex(int aChannelIndex)
{
    mChannelIndex = aChannelIndex;
    emit settingsChanged(QString("%1").arg(aChannelIndex));
}

bool SoundExtractChannelTransform::process()
{
    const SoundFrame *soundFrame = NULL;
    foreach (const FrameBase *frame, mCurrentFramesReadySet)
        if ((soundFrame = qobject_cast<const SoundFrame*>(frame)) != NULL)
            break;

    Q_ASSERT(soundFrame && (soundFrame->getChannelsNo() > 1));
    if (soundFrame->isValid() && (mChannelIndex < soundFrame->getChannelsNo()))
    {
        for (unsigned frame = 0; frame < soundFrame->getFramesNo(); ++frame)
            mChannelFrame.getSoundBuffer()[frame] = soundFrame->getSoundBuffer()[frame*soundFrame->getChannelsNo() + mChannelIndex];
        mChannelFrame.setFramesNo(soundFrame->getFramesNo());
        mChannelFrame.setSampleRate(soundFrame->getSampleRate());
        mChannelFrame.setTimeStamp(soundFrame->getTimeStamp());
        mChannelFrame.setSourceName(soundFrame->getSourceName());

        return true;
    }
    return false;
}

Q_EXPORT_PLUGIN2(PLUGINTARGETNAME, SoundExtractChannelTransformFactory);

} // namespace media
