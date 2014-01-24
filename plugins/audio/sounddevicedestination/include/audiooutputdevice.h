#ifndef AUDIOOUTPUTDEVICE_H
#define AUDIOOUTPUTDEVICE_H

#include <QIODevice>
#include "soundframe.h"

namespace media {

class AudioOutputDevice : public QIODevice
    {
    Q_OBJECT
public:
    explicit AudioOutputDevice(QObject *aParent = 0);

    bool open(OpenMode mode);
    void close();

    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;

    const SoundFrame& getSoundFrame() const { return mSoundFrame; }
    SoundFrame& getSoundFrame() { return mSoundFrame; }

signals:
    void bytesRead(int);

private:
    SoundFrame mSoundFrame;
    };

} // namespace media

#endif // AUDIOOUTPUTDEVICE_H
