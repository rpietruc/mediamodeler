#include "sounddevicedestination.h"
#include <QDebug>
#include <QAudioFormat>
#include <QAudioOutput>

namespace media {

SoundDeviceDestination::SoundDeviceDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mAudioOutput(NULL),
    mLastFrameTime(-1.0),
    mWaitForBytesRead(false)
    {
    setProperty("deviceName", "Default");
    QObject::connect(&mAudioOutputDevice, SIGNAL(bytesRead(int)), this, SLOT(checkBytesRead(int)), Qt::QueuedConnection);
    }

SoundDeviceDestination::~SoundDeviceDestination()
    {
    close();
    }

void SoundDeviceDestination::open()
    {
    Q_ASSERT(!mAudioOutput);

    foreach (QAudioDeviceInfo info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
        if (info.deviceName() == mAudioOutputDevice.getSoundFrame().getSourceName())
            {
            QAudioFormat format = info.preferredFormat();
            format.setSampleRate(1.0/mAudioOutputDevice.getSoundFrame().getDimensionT(SoundFrame::Time).mDelta);
            format.setChannelCount(mAudioOutputDevice.getSoundFrame().getDimensionT(SoundFrame::Channels).mResolution);
            if (info.isFormatSupported(format))
                {
                mAudioOutputDevice.getSoundFrame().setSampleBits(format.sampleSize());
                mAudioOutput = new QAudioOutput(info, format, this);
                QObject::connect(mAudioOutput, SIGNAL(notify()), SLOT(notified()));
                QObject::connect(mAudioOutput, SIGNAL(stateChanged(QAudio::State)), SLOT(stateChanged(QAudio::State)));
                mAudioOutputDevice.open(QIODevice::ReadOnly);
                }
            break;
            }
    }

void SoundDeviceDestination::close()
    {
    if (mAudioOutputDevice.isOpen())
        mAudioOutputDevice.close();

    if (mAudioOutput)
        {
//        if (mAudioOutput->state() != QAudio::StoppedState)
            mAudioOutput->stop();
        mAudioOutput->disconnect(this);
        delete mAudioOutput;
        mAudioOutput = NULL;
        }
    }

void SoundDeviceDestination::process()
    {
    if (mAudioOutputDevice.getSoundFrame().getSourceName() != property("deviceName").toString())
        {
        close();
        mAudioOutputDevice.getSoundFrame().setSourceName(property("deviceName").toString());
        }

    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == SoundFrame::Dimensions)
             && (frame->getDimensionT(SoundFrame::Time).mDelta > 0))
                {
                if (frame->getDimensionT(SoundFrame::Channels).mResolution != mAudioOutputDevice.getSoundFrame().getDimensionT(SoundFrame::Channels).mResolution)
                    {
                    close();
                    mAudioOutputDevice.getSoundFrame().setChannelsNo(frame->getDimensionT(SoundFrame::Channels).mResolution);
                    }
                if (frame->getDimensionT(SoundFrame::Time).mDelta != mAudioOutputDevice.getSoundFrame().getDimensionT(SoundFrame::Time).mDelta)
                    {
                    close();
                    mAudioOutputDevice.getSoundFrame().setSampleTime(frame->getDimensionT(SoundFrame::Time).mDelta);
                    }
                if (!isOpened())
                    open();

                if (frame->getDimensionT(SoundFrame::Time).mStartLocation > mLastFrameTime)
                    {
                    mAudioOutputDevice.getSoundFrame() += *frame;
                    qDebug() << "bytes free :" << mAudioOutputDevice.getSoundFrame().getBytesFree();
                    mWaitForBytesRead = (mAudioOutputDevice.getSoundFrame().getBytesFree() < SoundFrame::MaxFrameSize/2);
                    if (mWaitForBytesRead)
                        {
                        if (mAudioOutput->state() == QAudio::SuspendedState)
                            mAudioOutput->resume();
                        else if (mAudioOutput->state() == QAudio::StoppedState)
                            mAudioOutput->start(&mAudioOutputDevice);
                        }
                    }
                mLastFrameTime = frame->getDimensionT(SoundFrame::Time).mStartLocation;
                if (!mWaitForBytesRead)
                    emit framesProcessed();
                }
            }
    }

void SoundDeviceDestination::checkBytesRead(int aBytesRead)
    {
    if (mWaitForBytesRead && (aBytesRead > 0))
        {
        mWaitForBytesRead = (mAudioOutputDevice.getSoundFrame().getBytesFree() < SoundFrame::MaxFrameSize/2);
        if (!mWaitForBytesRead)
            emit framesReady();
        }
    }

void SoundDeviceDestination::stateChanged(QAudio::State aState)
    {
    qWarning() << "audio output: state = " << aState;
    if (aState == QAudio::IdleState)
        mAudioOutput->suspend();
    }

void SoundDeviceDestination::notified()
    {
//    qWarning() << "audio output: bytesFree = " << mAudioOutput->bytesFree()
//               << ", " << "elapsedUSecs = " << mAudioOutput->elapsedUSecs()
//               << ", " << "processedUSecs = " << mAudioOutput->processedUSecs();
    }

} // namespace media
