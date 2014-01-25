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
    mSoundFrame.setChannelsNo(1);
    mSoundFrame.setSampleBits(16);
    mSoundFrame.setSampleRate(8000);
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
    if (mAudioInput)
        open();

    if (!mInputDevice)
        {
        mInputDevice = mAudioInput->start();
        QObject::connect(mInputDevice, SIGNAL(readyRead()), SLOT(readMore()), Qt::QueuedConnection);
        }
    mSoundFrame.setFrameSize(0);
    qDebug() << "frame prepared";
    }

//ElementBase::ParamList SoundDeviceSource::getParams() const
//    {
//    Q_ASSERT(mSoundFrame.getDimension(SoundFrame::Time).mDelta);
//    ParamList ret;
//    ret["Audio Device"] = mSoundFrame.getSourceName();
//    ret["Sample Rate"] = 1.0/mSoundFrame.getDimension(SoundFrame::Time).mDelta;
//    ret["Channels No."] = mSoundFrame.getDimension(SoundFrame::Channels).mResolution;
//    return ret;
//    }

//void SoundDeviceSource::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    if (aName == "Audio Device")
//        {
//        if (mSoundFrame.getSourceName() != aValue.toString())
//            {
//            mSoundFrame.setSourceName(aValue.toString());
//            close();
//            }
//        }
//    else if (aName == "Sample Rate")
//        {
//        if (aValue.toInt() && (mSoundFrame.getDimension(SoundFrame::Time).mDelta != 1.0/aValue.toInt()))
//            {
//            mSoundFrame.setSampleRate(aValue.toInt());
//            close();
//            }
//        }
//    else if (aName == "Channels No")
//        {
//        if (mSoundFrame.getDimension(SoundFrame::Channels).mResolution != aValue.toInt())
//            {
//            mSoundFrame.setChannelsNo(aValue.toInt());
//            close();
//            }
//        }
//    }

void SoundDeviceSource::open()
    {
    if (mAudioInput)
        close();

    foreach (QAudioDeviceInfo info, QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
        if (info.deviceName() == mSoundFrame.getSourceName())
            {
            Q_ASSERT(mSoundFrame.getDimension(SoundFrame::Time).mDelta);
            QAudioFormat format = info.preferredFormat();
            format.setSampleRate(1.0/mSoundFrame.getDimension(SoundFrame::Time).mDelta);
            format.setChannelCount(mSoundFrame.getDimension(SoundFrame::Channels).mResolution);
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
    qWarning() << "audio input: state = " << state;
    }

void SoundDeviceSource::notified()
    {
    qWarning() << "audio input: bytesReady = " << mAudioInput->bytesReady()
               << ", " << "elapsedUSecs = " << mAudioInput->elapsedUSecs()
               << ", " << "processedUSecs = "<< mAudioInput->processedUSecs();
    }

} // namespace media
