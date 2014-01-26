#ifndef MEDIA_AUDIOFRAME_H
#define MEDIA_AUDIOFRAME_H

#include "framebase.h"

extern "C" {
    #include <libavformat/avformat.h>
    #include <libswresample/swresample.h>
}

namespace media {

class AudioFrame : public FrameBase
    {
public:
    enum { Time = 0, Channels, Dimensions };
    enum { LeftChannel = 0, RightChannel };

    explicit AudioFrame();
    virtual ~AudioFrame();

    qreal getSample(const int *aPoint) const;

    void allocateData(const AVCodecContext& aCodecContext);
    bool copyData(const AVFrame& aFrame);

private:
    void freeBuffers();

private:
    uint8_t **mBuffers;
    int mLineSize;
    int mMaxSamples;
    int mBufferSize;
    const AVSampleFormat mSampleFormat;
    SwrContext* mSwrContext;
    };

} // namespace media

#endif // MEDIA_AUDIOFRAME_H
