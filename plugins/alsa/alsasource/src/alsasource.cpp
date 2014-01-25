#include "alsasource.h"
#include <QDateTime>
#include <QDebug>

namespace media {

#define PCM_OPEN_MODE 0
//#define PCM_OPEN_MODE SND_PCM_NONBLOCK

AlsaSource::AlsaSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mPcmHandle(NULL)
    {
    setProperty("deviceName", "default");
    setProperty("sampleRate", 8000);
    setProperty("channelsNo", 2);
    }

AlsaSource::~AlsaSource()
    {
    close();
    }

void AlsaSource::open()
    {
    Q_ASSERT(!mPcmHandle);

    int res = 0;

    // Allocate a hardware parameters object
    snd_pcm_hw_params_t *hw_params;
    snd_pcm_hw_params_alloca(&hw_params);

    do
        {
        // Open PCM device
        res = snd_pcm_open(&mPcmHandle, qPrintable(mAlsaFrame.getSourceName()), SND_PCM_STREAM_CAPTURE, PCM_OPEN_MODE);
        if (res < 0)
            {
            mPcmHandle = NULL;
            break;
            }

        // Fill it in with default values
        res = snd_pcm_hw_params_any(mPcmHandle, hw_params);
        if (res < 0)
            break;

        // Set the desired hardware parameters
        res = snd_pcm_hw_params_set_access(mPcmHandle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
        if (res < 0)
            break;

        res = snd_pcm_hw_params_set_format(mPcmHandle, hw_params, SND_PCM_FORMAT_S16_LE);
        if (res < 0)
            break;

        res = snd_pcm_hw_params_set_channels(mPcmHandle, hw_params, mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution);
        if (res < 0)
            break;

        int dir = 0;
        Q_ASSERT(mAlsaFrame.getDimension(AlsaFrame::Time).mDelta);
        int rate = 1.0/mAlsaFrame.getDimension(AlsaFrame::Time).mDelta;
        res = snd_pcm_hw_params_set_rate(mPcmHandle, hw_params, rate, dir);
        if (res < 0)
            break;

        dir = 0;
        snd_pcm_uframes_t frameSize = rate*AlsaFrame::DefaultFrameTime;
        res = snd_pcm_hw_params_set_period_size_near(mPcmHandle, hw_params, &frameSize, &dir);
        if (res < 0)
            break;
        mAlsaFrame.setFrameTime(static_cast<double>(frameSize)/static_cast<double>(rate));

        // Write the parameters to the driver
        res = snd_pcm_hw_params(mPcmHandle, hw_params);
        if (res < 0)
            break;

        dir = 0;
        unsigned int periodTime;
        res = snd_pcm_hw_params_get_period_time(hw_params, &periodTime, &dir);
        }
    while (0);

    if ((res < 0) && mPcmHandle)
        {
        snd_pcm_close(mPcmHandle);
        mPcmHandle = NULL;
        }
//    snd_pcm_hw_params_free(hw_params);
    }

void AlsaSource::close()
    {
    if (mPcmHandle)
        {
        snd_pcm_drain(mPcmHandle);
        snd_pcm_close(mPcmHandle);
        }
    mPcmHandle = NULL;
    }

void AlsaSource::process()
    {
    if (mAlsaFrame.getSourceName() != property("deviceName").toString())
        {
        close();
        mAlsaFrame.setSourceName(property("deviceName").toString());
        qDebug() << objectName() << ": deviceName changed to " << mAlsaFrame.getSourceName();
        }
    if (mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution != property("channelsNo").toInt())
        {
        close();
        mAlsaFrame.setChannelsNo(property("channelsNo").toInt());
        qDebug() << objectName() << ": channelsNo changed to " << mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution;
        }
    if (property("sampleRate").toInt() &&
        (mAlsaFrame.getDimension(AlsaFrame::Time).mDelta != 1.0/property("sampleRate").toInt()))
        {
        close();
        mAlsaFrame.setSampleRate(property("sampleRate").toInt());
        qDebug() << objectName() << ": sampleRate changed to " << 1.0/mAlsaFrame.getDimension(AlsaFrame::Time).mDelta;
        }

    snd_pcm_sframes_t ret = 0;
    if (!mPcmHandle)
        open();

    if (mPcmHandle)
        {
        ret = snd_pcm_readi(mPcmHandle, mAlsaFrame.getSoundBuffer(), mAlsaFrame.getDimension(AlsaFrame::Time).mResolution);
        if (ret <= 0)
            qWarning() << "snd_pcm_readi returned " << ret;
        if (ret == -EPIPE)
            {
            /* EPIPE means xrun (overrun for capture)
               The overrun can happen when an application does not take new captured samples in time from alsa-lib. */
            snd_pcm_prepare(mPcmHandle);
            ret = snd_pcm_readi(mPcmHandle, mAlsaFrame.getSoundBuffer(), mAlsaFrame.getDimension(AlsaFrame::Time).mResolution);
            }
        if (ret > 0)
            mAlsaFrame.setTimeStamp(QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0);
        }

    if (ret > 0)
        emit framesReady();
    else
        emit processingCompleted();
    }

} // namespace media
