#include "audiooutputdevice.h"
#include <QtEndian>

using namespace media;

AudioOutputDevice::AudioOutputDevice(QObject *parent) :
    QIODevice(parent)
    {
    mSoundFrame.setSampleBits(16);
    }

bool AudioOutputDevice::open(OpenMode mode)
    {
    Q_UNUSED(mode);
    return QIODevice::open(QIODevice::ReadOnly);
    }

void AudioOutputDevice::close()
    {
    QIODevice::close();
    }

qint64 AudioOutputDevice::readData(char *data, qint64 maxlen)
    {
    qint16* ptr = reinterpret_cast<qint16 *>(data);
    int sample = 0;
    int point[SoundFrame::Dimensions] = {0, 0};
    for (point[SoundFrame::Time] = 0;
         (point[SoundFrame::Time] < mSoundFrame.getDimension(SoundFrame::Time).mResolution) &&
         ((qint64)(sample*sizeof(qint16)) < maxlen);
         ++point[SoundFrame::Time])
        for (point[SoundFrame::Channels] = 0;
             (point[SoundFrame::Channels] < mSoundFrame.getDimension(SoundFrame::Channels).mResolution) &&
             ((qint64)(sample*sizeof(qint16)) < maxlen);
             ++point[SoundFrame::Channels])
            ptr[sample++] = qToLittleEndian(static_cast<qint16>(mSoundFrame.getSample(point)*32767.0));

    if (sample > 0)
        {
        mSoundFrame.eatBytes(sample*sizeof(qint16));
        emit bytesRead(sample*sizeof(qint16));
        }
    return sample*sizeof(qint16);
    }

qint64 AudioOutputDevice::writeData(const char *data, qint64 len)
    {
    Q_UNUSED(data);
    Q_UNUSED(len);

    return 0;
    }

qint64 AudioOutputDevice::bytesAvailable() const
    {
    qint64 ret = QIODevice::bytesAvailable();
    return mSoundFrame.getDimension(SoundFrame::Channels).mResolution*mSoundFrame.getDimension(SoundFrame::Time).mResolution*sizeof(qint16) + ret;
    }
