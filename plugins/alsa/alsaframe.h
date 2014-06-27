#ifndef ALSAFRAME_H
#define ALSAFRAME_H

#include "framebase.h"
#include <QObject>

namespace media {

class AlsaFrame : public QObject, public FrameBase
    {
    Q_OBJECT

public:
    enum { Time = 0, Channels, Dimensions };
    enum { LeftChannel = 0, RightChannel };

//    static const int DefaultSampleRate = 8000;
//    static const int DefaultChannelsNo = 1;
    static const double DefaultFrameTime = 0.02; //20 ms

    explicit AlsaFrame(QObject *aParent = 0);
    virtual ~AlsaFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    void resize(const int *aSize)
        {
        mDimensions[Time].mResolution = aSize[Time];
        setChannelsNo(aSize[Channels]);
        }

    void operator+=(const FrameBase &aFrame);
    const qint16 *getSoundBuffer() const { return mSoundBuffer; }
    qint16 *getSoundBuffer() { return mSoundBuffer; }

    void setChannelsNo(int aChannelsNo);
    void setSampleTime(double aSampleTime);
    void setFrameTime(double aFrameTime);

    void incrementTimeStamp();
    void setTimeStamp(double aTimeStamp);

signals:
    void bufferFilled();

protected:
    void resizeSoundBuffer();

private:
    qint16 *mSoundBuffer;
    int mBufSize;
    };

} // namespace media

#endif // ALSAFRAME_H
