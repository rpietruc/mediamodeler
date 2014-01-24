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
    mAudioOutputDevice.getSoundFrame().setSourceName("Default");
    QObject::connect(&mAudioOutputDevice, SIGNAL(bytesRead(int)), this, SLOT(checkBytesRead(int)), Qt::QueuedConnection);
    }

SoundDeviceDestination::~SoundDeviceDestination()
    {
    close();
    }

ElementBase::ParamList SoundDeviceDestination::getParams() const
    {
    ParamList ret;
//    mAudioDeviceInfo(QAudioDeviceInfo::defaultOutputDevice()),
//    ret["Audio Device"] = mAudioDeviceInfo.deviceName();
    ret["Audio Device"] = mAudioOutputDevice.getSoundFrame().getSourceName();
    return ret;
    }

void SoundDeviceDestination::setParamValue(const QString& aName, const QVariant& aValue)
    {
    Q_UNUSED(aName);
    if (mAudioOutputDevice.getSoundFrame().getSourceName() != aValue.toString())
        {
        close();
        mAudioOutputDevice.getSoundFrame().setSourceName(aValue.toString());
        }
    }

void SoundDeviceDestination::open()
    {
    close();

    foreach (QAudioDeviceInfo info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
        if (info.deviceName() == mAudioOutputDevice.getSoundFrame().getSourceName())
            {
            QAudioFormat format = info.preferredFormat();
            format.setSampleRate(1.0/mAudioOutputDevice.getSoundFrame().getDimension(SoundFrame::Time).mDelta);
            format.setChannelCount(mAudioOutputDevice.getSoundFrame().getDimension(SoundFrame::Channels).mResolution);
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
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == SoundFrame::Dimensions)
             && (frame->getDimension(SoundFrame::Time).mDelta > 0))
                {
                if (frame->getDimension(SoundFrame::Channels).mResolution != mAudioOutputDevice.getSoundFrame().getDimension(SoundFrame::Channels).mResolution)
                    {
                    close();
                    mAudioOutputDevice.getSoundFrame().setChannelsNo(frame->getDimension(SoundFrame::Channels).mResolution);
                    }
                if (frame->getDimension(SoundFrame::Time).mDelta != mAudioOutputDevice.getSoundFrame().getDimension(SoundFrame::Time).mDelta)
                    {
                    close();
                    mAudioOutputDevice.getSoundFrame().setSampleRate(1.0/frame->getDimension(SoundFrame::Time).mDelta);
                    }
                if (!isOpened())
                    open();

                if (frame->getDimension(SoundFrame::Time).mStartLocation > mLastFrameTime)
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
                mLastFrameTime = frame->getDimension(SoundFrame::Time).mStartLocation;
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
