#include "sounddevicesource.h"
#include <QDebug>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QDateTime>

namespace media {

SoundDeviceSource::SoundDeviceSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mAudioInput(NULL),
    mInputDevice(NULL)
    {
    setProperty("deviceName", "Defult");
    setProperty("sampleRate", 8000);
    setProperty("channelsNo", 1);
    }

SoundDeviceSource::~SoundDeviceSource()
    {
    close();
    }

void SoundDeviceSource::readMore()
    {
    Q_ASSERT(mAudioInput);
    qint64 len = mAudioInput->bytesReady();
    if ((len >  0) && !mSoundFrame.getFrameSize())
        {
        if (len > mSoundFrame.getBytesFree())
            len = mSoundFrame.getBytesFree();

        qint64 res = mInputDevice->read(mSoundFrame.getSoundBuffer(), len);
        Q_ASSERT(res > 0);
        mSoundFrame.setFrameSize(res);
        mSoundFrame.setTimeStamp(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);
        qDebug() << "frame processed";
        emit framesReady();
        }
    }

void SoundDeviceSource::process()
    {
    if (mSoundFrame.getSourceName() != property("deviceName").toString())
        {
        close();
        mSoundFrame.setSourceName(property("deviceName").toString());
        }
    if (property("sampleRate").toInt() && (mSoundFrame.getDimensionT(SoundFrame::Time).mDelta != 1.0/property("sampleRate").toInt()))
        {
        close();
        mSoundFrame.setSampleTime(1.0/property("sampleRate").toInt());
        }
    if (mSoundFrame.getDimensionT(SoundFrame::Channels).mResolution != property("channelsNo").toInt())
        {
        close();
        mSoundFrame.setChannelsNo(property("channelsNo").toInt());
        }
    mSoundFrame.setSampleBits(16);

    if (!mAudioInput)
        open();

    if (!mInputDevice)
        {
        mInputDevice = mAudioInput->start();
        QObject::connect(mInputDevice, SIGNAL(readyRead()), SLOT(readMore()), Qt::QueuedConnection);
        }
    mSoundFrame.setFrameSize(0);
//    qDebug() << "frame prepared";
    }

void SoundDeviceSource::open()
    {
    if (mAudioInput)
        close();

    foreach (QAudioDeviceInfo info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
        if (info.deviceName() == mSoundFrame.getSourceName())
            {
            Q_ASSERT(mSoundFrame.getDimensionT(SoundFrame::Time).mDelta);
            QAudioFormat format = info.preferredFormat();
            format.setSampleRate(1.0/mSoundFrame.getDimensionT(SoundFrame::Time).mDelta);
            format.setChannelCount(mSoundFrame.getDimensionT(SoundFrame::Channels).mResolution);
//            format.setSampleType(QAudioFormat::SignedInt);
//            format.setByteOrder(QAudioFormat::LittleEndian);
//            format.setCodec("audio/pcm");
            if (info.isFormatSupported(format))
                break;

            mAudioInput = new QAudioInput(info, format, this);
            QObject::connect(mAudioInput, SIGNAL(notify()), SLOT(notified()));
            QObject::connect(mAudioInput, SIGNAL(stateChanged(QAudio::State)), SLOT(stateChanged(QAudio::State)));
            mInputDevice = NULL;

            break;
            }
    }

void SoundDeviceSource::close()
    {
    if (mAudioInput)
        {
        mAudioInput->stop();
        mAudioInput->disconnect(this);
        delete mAudioInput;
        }
    mAudioInput = NULL;
    }

void SoundDeviceSource::stateChanged(QAudio::State state)
    {
//    qWarning() << "audio input: state = " << state;
    }

void SoundDeviceSource::notified()
    {
//    qWarning() << "audio input: bytesReady = " << mAudioInput->bytesReady()
//               << ", " << "elapsedUSecs = " << mAudioInput->elapsedUSecs()
//               << ", " << "processedUSecs = "<< mAudioInput->processedUSecs();
    }

} // namespace media
