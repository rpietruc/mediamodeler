#include "audioframe.h"

extern "C" {
    #include <libavutil/opt.h>
    #include <libavutil/channel_layout.h>
    #include <libavutil/samplefmt.h>
    #include <libswresample/swresample.h>
}

namespace media {

AudioFrame::AudioFrame() :
    FrameBase(Dimensions),
    mBuffers(NULL),
    mLineSize(0),
    mMaxSamples(0),
    mBufferSize(0),
    mSampleFormat(AV_SAMPLE_FMT_FLTP),
    mSwrContext(NULL)
    {
    mDimensions[Channels].mDelta = 1;
    mSwrContext = swr_alloc();
    Q_ASSERT(mSwrContext);
    Q_ASSERT(sizeof(float) == av_get_bytes_per_sample(mSampleFormat));
    }

AudioFrame::~AudioFrame()
    {
    swr_free(&mSwrContext);
    freeBuffers();
    av_freep(&mBuffers);
    mBuffers = NULL;
    }

double AudioFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(aPoint[Channels] < mDimensions[Channels].mResolution);
    Q_ASSERT(aPoint[Time] < mMaxSamples);
    float *ptr = reinterpret_cast<float *>(mBuffers[aPoint[Channels]]);
    return ptr[aPoint[Time]];
    }

void AudioFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(aPoint[Channels] < mDimensions[Channels].mResolution);
    Q_ASSERT(aPoint[Time] < mMaxSamples);
    float *ptr = reinterpret_cast<float *>(mBuffers[aPoint[Channels]]);
    ptr[aPoint[Time]] = aValue;
    }

void AudioFrame::allocateData(const AVCodecContext& aCodecContext)
    {
    Q_ASSERT(aCodecContext.sample_rate);
    freeBuffers();
    av_freep(&mBuffers);
    mBuffers = reinterpret_cast<uint8_t **>(av_mallocz(sizeof(uint8_t *) * mDimensions[Channels].mResolution));

    mDimensions[Channels].mResolution = aCodecContext.channels;
    mDimensions[Time].mDelta = 1.0/aCodecContext.sample_rate;

    av_opt_set_int(mSwrContext, "in_channel_layout", aCodecContext.channel_layout, 0);
    av_opt_set_int(mSwrContext, "out_channel_layout", aCodecContext.channel_layout, 0);

    av_opt_set_int(mSwrContext, "in_sample_rate", aCodecContext.sample_rate, 0);
    av_opt_set_int(mSwrContext, "out_sample_rate", aCodecContext.sample_rate, 0);

    av_opt_set_sample_fmt(mSwrContext, "in_sample_fmt", aCodecContext.sample_fmt, 0);
    av_opt_set_sample_fmt(mSwrContext, "out_sample_fmt", mSampleFormat, 0);

    int ret = swr_init(mSwrContext);
    Q_ASSERT(ret >= 0);
    }

void AudioFrame::resize(const int *aSize)
    {
    freeBuffers();
    av_freep(&mBuffers);
    av_samples_alloc(mBuffers, &mLineSize, aSize[Channels], aSize[Time], mSampleFormat, 1);
    mMaxSamples = aSize[Time];
    mBufferSize = av_samples_get_buffer_size(&mLineSize, aSize[Channels], aSize[Time], mSampleFormat, 1);
    mBuffers = reinterpret_cast<uint8_t **>(av_mallocz(sizeof(uint8_t *) * aSize[Channels]));
    }

bool AudioFrame::copyData(const AVFrame& aFrame)
    {
    mDimensions[Time].mStartLocation += mDimensions[Time].mResolution*mDimensions[Time].mDelta;
    mDimensions[Time].mResolution = aFrame.nb_samples;
    if (mMaxSamples < mDimensions[Time].mResolution)
        {
        freeBuffers();
        av_samples_alloc(mBuffers, &mLineSize, mDimensions[Channels].mResolution, mDimensions[Time].mResolution, mSampleFormat, 1);
        mMaxSamples = mDimensions[Time].mResolution;
        mBufferSize = av_samples_get_buffer_size(&mLineSize, mDimensions[Channels].mResolution, mDimensions[Time].mResolution, mSampleFormat, 1);
        }
    // convert to destination format
    Q_ASSERT(mBuffers);
    int ret = swr_convert(mSwrContext, mBuffers, mMaxSamples, const_cast<const uint8_t **>(aFrame.extended_data), aFrame.nb_samples);
    return ret > 0;
    }

void AudioFrame::freeBuffers()
    {
    if ((mMaxSamples > 0) && mBuffers)
        {
        //The allocated samples buffer can be freed by using av_freep(&mBuffers[0])
        if ((mDimensions[Channels].mResolution > 0) && mBuffers[0])
            av_freep(&mBuffers[0]);
        mMaxSamples = 0;
        mBufferSize = 0;
        }
    }

} // namespace media
