#ifndef MEDIA_SOUNDFRAME_H
#define MEDIA_SOUNDFRAME_H

#include "framebase.h"
#include <QAudioFormat>

namespace media {

class SoundFrame : public QObject, public FrameBase
    {
    Q_OBJECT

public:
    enum { Time = 0, Channels, Dimensions };
    enum { LeftChannel = 0, RightChannel };

    static const int MaxFrameSize = 65536;

    explicit SoundFrame(QObject *aParent = 0);

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    inline char* getSoundBuffer() { return mSoundBuffer.data(); }

    void setFrameSize(int aBytesNo);
    void setFrameSamples(int aVectorNo);
    bool incrementFrameTime();
    int getFrameSize() const ;
    int getBytesFree() const { return mSoundBuffer.size() - getFrameSize(); }

    void setSampleBits(int aSampleBits);
    int getSampleBits() const { return mSampleBits; }

    void setSampleTime(double aSampleTime);
    void setChannelsNo(int aChannelsNo);

    inline void setTimeStamp(double aTimeStamp) { mDimensions[Time].mStartLocation = aTimeStamp; }
    void eatBytes(int aSize);
    void operator+=(const FrameBase &aFrame);
    const SoundFrame& operator=(const FrameBase &aFrame);

signals:
    void bufferFilled();

private:
    int mSampleBits;
    QByteArray mSoundBuffer;
    };

} // namespace media

#endif // MEDIA_SOUNDFRAME_H
