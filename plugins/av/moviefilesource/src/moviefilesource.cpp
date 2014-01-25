#include "moviefilesource.h"
#include <QDebug>

extern "C" {
#include <libavformat/avformat.h>
//#include <libavutil/timestamp.h>
}

namespace media {

MovieFileSource::MovieFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mFormatContext(NULL),
    mAudioStreamIdx(-1),
    mVideoStreamIdx(-1),
    mVideoStream(NULL),
    mAudioStream(NULL),
    mAudioDecoderContext(NULL),
    mVideoDecoderContext(NULL),
    mFrame(NULL),
    mVideoFrameCount(0),
    mAudioFrameCount(0),
    mCached(false)
    {
    // register all formats and codecs
    av_register_all();
    mFrame = avcodec_alloc_frame();
    Q_ASSERT(mFrame);
    /* initialize packet, set data to NULL, let the demuxer fill it */
    av_init_packet(&mPacket);
    mPacket.data = NULL;
    mPacket.size = 0;
    }

MovieFileSource::~MovieFileSource()
    {
    if (mVideoDecoderContext)
        avcodec_close(mVideoDecoderContext);
    if (mAudioDecoderContext)
        avcodec_close(mAudioDecoderContext);
    if (mFormatContext)
        avformat_close_input(&mFormatContext);
    if (mFrame)
        av_free(mFrame);
    }

//ElementBase::ParamList MovieFileSource::getParams() const
//    {
//    ParamList ret;
//    if (mVideoFrame.getSourceName().isEmpty())
//        ret["File"] = mAudioFrame.getSourceName();
//    else
//        ret["File"] = mVideoFrame.getSourceName();
//    return ret;
//    }

//void MovieFileSource::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    Q_UNUSED(aName);

//    if (mVideoDecoderContext)
//        avcodec_close(mVideoDecoderContext);
//    if (mAudioDecoderContext)
//        avcodec_close(mAudioDecoderContext);
//    if (mFormatContext)
//        avformat_close_input(&mFormatContext);

//    // open input file, and allocate format context
//    if (avformat_open_input(&mFormatContext, qPrintable(aValue.toString()), NULL, NULL) >= 0)
//        {
//        // retrieve stream information
//        if (avformat_find_stream_info(mFormatContext, NULL) >= 0)
//            {
//            if (openCodecContext(&mVideoStreamIdx, mFormatContext, AVMEDIA_TYPE_VIDEO) >= 0)
//                {
//                mVideoStream = mFormatContext->streams[mVideoStreamIdx];
//                mVideoFrame.allocateData(*mVideoStream->codec);
//                mVideoDecoderContext = mVideoStream->codec;
//                mVideoFrame.setSourceName(aValue.toString());
//                }
//            if (openCodecContext(&mAudioStreamIdx, mFormatContext, AVMEDIA_TYPE_AUDIO) >= 0)
//                {
//                mAudioStream = mFormatContext->streams[mAudioStreamIdx];
//                mAudioFrame.allocateData(*mAudioStream->codec);
//                mAudioDecoderContext = mAudioStream->codec;
//                mAudioFrame.setSourceName(aValue.toString());
//                }
//            // if any of stream opened
//            if (mAudioDecoderContext || mVideoDecoderContext)
//                {
//                // dump input information to stderr
//                av_dump_format(mFormatContext, 0, qPrintable(aValue.toString()), 0);
//                }
//            }
//        else
//            qWarning() << "Could not find stream information " << aValue.toString();
//        }
//    else
//        qWarning() << "Could not open source file " << aValue.toString();
//    }

int MovieFileSource::openCodecContext(int *aStreamIdx, AVFormatContext *aFormatContext, enum AVMediaType aMediaType)
    {
    int ret = av_find_best_stream(aFormatContext, aMediaType, -1, -1, NULL, 0);
    if (ret < 0)
        {
        qWarning() << "Could not find " << av_get_media_type_string(aMediaType) << " stream in input file";
        return ret;
        }
    else
        {
        *aStreamIdx = ret;
        AVStream *st = aFormatContext->streams[*aStreamIdx];

        // find decoder for the stream
        AVCodecContext *dec_ctx = st->codec;
        AVCodec *dec = avcodec_find_decoder(dec_ctx->codec_id);
        if (!dec)
            {
            qWarning() << "Failed to find " << av_get_media_type_string(aMediaType) << " codec";
            return ret;
            }
        if ((ret = avcodec_open2(dec_ctx, dec, NULL)) < 0)
            {
            qWarning() << "Failed to open " << av_get_media_type_string(aMediaType) << " codec";
            return ret;
            }
        }
    return 0;
    }

void MovieFileSource::process()
    {
    bool ret = false;
    if (mAudioDecoderContext || mVideoDecoderContext)
        {
        ret = true;
        int gotFrame = 0;

        while (!gotFrame && ret)
            {
            int res = av_read_frame(mFormatContext, &mPacket);
            if ((res < 0) && !mCached)
                {
                // flush cached frames
                mPacket.data = NULL;
                mPacket.size = 0;
                mCached = true;
                }

            if (res < 0 && !mCached)
               ret = false;

            else if (mPacket.stream_index == mVideoStreamIdx)
                {
                // decode video frame
                int ret = avcodec_decode_video2(mVideoDecoderContext, mFrame, &gotFrame, &mPacket);
                if (ret >= 0)
                    {
                    if (gotFrame)
                        {
                        //qDebug () << "video frame no. " << mVideoFrameCount++
                                  //<< ", cached " << mCached
                                  //<< ", coded no. " << mFrame->coded_picture_number;
                                  //<< ", pts " << av_ts2timestr(mFrame->pts, &mVideoDecoderContext->time_base);
                        if (mVideoFrame.copyData(*mFrame))
                            ret = true;
                        }
                    }
                else
                    qWarning("Error decoding video frame");
                }
            else if (mPacket.stream_index == mAudioStreamIdx)
                {
                // decode audio frame
                int ret = avcodec_decode_audio4(mAudioDecoderContext, mFrame, &gotFrame, &mPacket);
                    {
                    if (ret >= 0)
                        {
                        if (gotFrame)
                            {
                            //qDebug () << "audio frame no. " << mAudioFrameCount++
                                      //<< ", cached " << mCached
                                      //<< ", samples " << mFrame->nb_samples;
                                      //<< ", pts " << av_ts2timestr(mFrame->pts, &mAudioDecoderContext->time_base);
                            if (mAudioFrame.copyData(*mFrame))
                                ret = true;
                            }
                        }
                    else
                        qWarning("Error decoding audio frame");
                    }
                }
            av_free_packet(&mPacket);
            }
        }

    if (ret)
        emit framesReady();
    else
        emit processingCompleted();
    }

} // namespace media
