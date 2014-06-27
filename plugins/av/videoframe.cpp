#include "videoframe.h"

extern "C" {
    #include <libavutil/imgutils.h>
    #include <libavutil/pixdesc.h>
    #include <libavcodec/avcodec.h>
}

namespace media {

VideoFrame::VideoFrame() :
    FrameBase(Dimensions),
    mVideoDataBuffer(NULL),
    mVideoBufferSize(0),
    mPixelFormat(AV_PIX_FMT_BGR24),
    mSwsContext(NULL)
    {
    mDimensions[Width].mDelta = 1;
    mDimensions[Height].mDelta = 1;
    mDimensions[Channels].mDelta = 1;
    mDimensions[Channels].mResolution = 3;
    mRgbFrame = av_frame_alloc();
    }

VideoFrame::~VideoFrame()
    {
    if (mSwsContext)
        sws_freeContext(mSwsContext);
    if (mVideoDataBuffer)
        av_free(mVideoDataBuffer);
    }

void VideoFrame::resize(const int* aPoint)
    {
    // allocate image where the decoded image will be put
    if (mVideoDataBuffer)
        av_free(mVideoDataBuffer);

    mVideoBufferSize = avpicture_get_size(mPixelFormat, aPoint[Width], aPoint[Height]);
    mVideoDataBuffer = reinterpret_cast<uint8_t *>(av_malloc(mVideoBufferSize*sizeof(uint8_t)));
    mDimensions[Width].mResolution = aPoint[Width];
    mDimensions[Height].mResolution = aPoint[Height];
    avpicture_fill(reinterpret_cast<AVPicture *>(mRgbFrame), mVideoDataBuffer, mPixelFormat, mDimensions[Width].mResolution, mDimensions[Height].mResolution);
    Q_ASSERT(0 && "decide  pix format");
    //mSwsContext = sws_getContext(mDimensions[Width].mResolution, mDimensions[Height].mResolution, /*aCodecContext.pix_fmt*/,
      //                           mDimensions[Width].mResolution, mDimensions[Height].mResolution, mPixelFormat, SWS_BICUBIC, NULL, NULL, NULL);
    }

void VideoFrame::allocateData(const AVCodecContext& aCodecContext)
    {
    // allocate image where the decoded image will be put
    if (mVideoDataBuffer)
        av_free(mVideoDataBuffer);

    mVideoBufferSize = avpicture_get_size(mPixelFormat, aCodecContext.width, aCodecContext.height);
    mVideoDataBuffer = reinterpret_cast<uint8_t *>(av_malloc(mVideoBufferSize*sizeof(uint8_t)));
    mDimensions[Width].mResolution = aCodecContext.width;
    mDimensions[Height].mResolution = aCodecContext.height;
    avpicture_fill(reinterpret_cast<AVPicture *>(mRgbFrame), mVideoDataBuffer, mPixelFormat, mDimensions[Width].mResolution, mDimensions[Height].mResolution);
    mSwsContext = sws_getContext(mDimensions[Width].mResolution, mDimensions[Height].mResolution, aCodecContext.pix_fmt,
                                 mDimensions[Width].mResolution, mDimensions[Height].mResolution, mPixelFormat, SWS_BICUBIC, NULL, NULL, NULL);
    }

bool VideoFrame::copyData(const AVFrame& aFrame)
    {
    Q_ASSERT(mVideoDataBuffer);
    int ret = sws_scale(mSwsContext, aFrame.data, aFrame.linesize, 0, aFrame.height, mRgbFrame->data, mRgbFrame->linesize);
    return ret > 0;
    }

qreal VideoFrame::getSampleT(const int *aPoint) const
    {
    double value = 0.0;
    int offset = (aPoint[Width] + aPoint[Height]*mDimensions[Width].mResolution)*mDimensions[Channels].mResolution + aPoint[Channels];
    Q_ASSERT(offset < mVideoBufferSize);
    value = mVideoDataBuffer[offset];
    return value;
    }

void VideoFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    int offset = (aPoint[Width] + aPoint[Height]*mDimensions[Width].mResolution)*mDimensions[Channels].mResolution + aPoint[Channels];
    Q_ASSERT(offset < mVideoBufferSize);
    mVideoDataBuffer[offset] = aValue;
    }


} // namespace media
