#ifndef MEDIA_VIDEOFRAME_H
#define MEDIA_VIDEOFRAME_H

#include "framebase.h"

extern "C" {
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
}

namespace media {

class VideoFrame : public FrameBase
    {
public:
    enum { Width, Height, Channels, Dimensions };

    explicit VideoFrame();
    virtual ~VideoFrame();

    qreal getSampleT(const int *aPoint) const;

    void allocateData(const AVCodecContext& aCodecContext);
    bool copyData(const AVFrame& aFrame);

private:
    uint8_t *mVideoDataBuffer;
    int mVideoBufferSize;
    const AVPixelFormat mPixelFormat;
    AVFrame *mRgbFrame;
    SwsContext *mSwsContext;
    };

} // namespace media

#endif // MEDIA_VIDEOFRAME_H
