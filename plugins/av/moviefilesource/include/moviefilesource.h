#ifndef MOVIEFILESOURCE_H
#define MOVIEFILESOURCE_H

#include "elementbase.h"
#include "audioframe.h"
#include "videoframe.h"
#include <QString>
#include <QtCore/qglobal.h>

extern "C" {
#include <libavformat/avformat.h>
}

#if defined(MOVIEFILESOURCE_LIBRARY)
#  define MOVIEFILESOURCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define MOVIEFILESOURCESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class MOVIEFILESOURCESHARED_EXPORT MovieFileSource : public ElementBase
    {
    Q_OBJECT

public:
    explicit MovieFileSource(ElementFactory *aFactory, const QString &aObjectName);
    ~MovieFileSource();

    int getFramesNo() const { return 2; }
    const FrameBase *getFrame(int aIndex) const
        {
        if (aIndex)
            return &mAudioFrame;
        //else
            return &mVideoFrame;
        }

private slots:
    void open();

private:
    static int openCodecContext(int *aStreamIdx, AVFormatContext *aFormatContext, enum AVMediaType aMediaType);
    void process();

private:
    AudioFrame mAudioFrame;
    VideoFrame mVideoFrame;

    AVFormatContext *mFormatContext;
    int mAudioStreamIdx;
    int mVideoStreamIdx;
    AVStream *mVideoStream;
    AVStream *mAudioStream;
    AVCodecContext *mAudioDecoderContext;
    AVCodecContext *mVideoDecoderContext;
    AVFrame *mFrame;
    AVPacket mPacket;
    int mVideoFrameCount;
    int mAudioFrameCount;
    bool mCached;
    };

class MovieFileSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "MovieFileSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new MovieFileSource(this, aObjectName); }
    };

} // namespace media

#endif // MOVIEFILESOURCE_H
