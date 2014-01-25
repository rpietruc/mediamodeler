#include "alsadestination.h"
#include <QDebug>
#include <alsa/asoundlib.h>

namespace media {

#define PCM_OPEN_MODE 0
//#define PCM_OPEN_MODE SND_PCM_NONBLOCK

AlsaDestination::AlsaDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mPcmHandle(NULL)
    {
    setDeviceName("default");
    QObject::connect(this, SIGNAL(deviceNameChanged()), this, SLOT(close()), Qt::QueuedConnection);
    QObject::connect(&mAlsaFrame, SIGNAL(bufferFilled()), this, SLOT(write()));
    }

AlsaDestination::~AlsaDestination()
    {
    close();
    }

void AlsaDestination::open()
    {
    Q_ASSERT(!mPcmHandle);

    // Open PCM device
    int res = snd_pcm_open(&mPcmHandle, qPrintable(mAlsaFrame.getSourceName()), SND_PCM_STREAM_PLAYBACK, PCM_OPEN_MODE);
    if (res < 0)
        qWarning() << "snd_pcm_open failed" << mAlsaFrame.getSourceName();
    else
        {
        // Allocate a hardware parameters object
        snd_pcm_hw_params_t *hw_params;
        snd_pcm_hw_params_alloca(&hw_params);

        // Fill it in with default values
        res = snd_pcm_hw_params_any(mPcmHandle, hw_params);
        if (res < 0)
            qWarning() << "snd_pcm_hw_params_any failed";
        else
            {
            // Set the desired hardware parameters
            snd_pcm_access_t access = SND_PCM_ACCESS_RW_INTERLEAVED;
            res = snd_pcm_hw_params_set_access(mPcmHandle, hw_params, access);
            if (res < 0)
                qWarning() << "snd_pcm_hw_params_set_access failed" << SND_PCM_ACCESS_RW_INTERLEAVED;
            else
                {
                snd_pcm_format_t val = SND_PCM_FORMAT_S16_LE;
                res = snd_pcm_hw_params_set_format(mPcmHandle, hw_params, val);
                if (res < 0)
                    qWarning() << "snd_pcm_hw_params_set_format failed" << val;
                }
            }
        if (res < 0)
            {
            snd_pcm_close(mPcmHandle);
            mPcmHandle = NULL;
            }
        else
            {
            res = snd_pcm_hw_params_set_channels(mPcmHandle, hw_params, mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution);
            if (res < 0)
                qWarning() << "channels not supported" << mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution;
            else
                {
                Q_ASSERT(mAlsaFrame.getDimension(AlsaFrame::Time).mDelta >= 0);
                unsigned int rate = 1.0/mAlsaFrame.getDimension(AlsaFrame::Time).mDelta;
                int dir = 0;
                res = snd_pcm_hw_params_set_rate(mPcmHandle, hw_params, rate, dir);
                if (res < 0)
                    qWarning() << "sample rate not supported" << rate;
                else
                    {
                    mAlsaFrame.setFrameTime(AlsaFrame::DefaultFrameTime);
                    snd_pcm_uframes_t frameSize = mAlsaFrame.getDimension(AlsaFrame::Time).mResolution;
                    dir = 0;
                    res = snd_pcm_hw_params_set_period_size_near(mPcmHandle, hw_params, &frameSize, &dir);
                    if (res < 0)
                        qWarning() << "frame size not supported" << frameSize;
                    else
                        {
                        // Write the parameters to the driver
                        res = snd_pcm_hw_params(mPcmHandle, hw_params);
                        if (res < 0)
                            qWarning() << "snd_pcm_hw_params failed";
                        }
                    }
                }
            }
//        snd_pcm_hw_params_free(hw_params);
        }
    }

void AlsaDestination::close()
    {
    if (mPcmHandle)
        {
        snd_pcm_drain(mPcmHandle);
        snd_pcm_close(mPcmHandle);
        }
    mPcmHandle = NULL;
    }

void AlsaDestination::write()
    {
    if (!mPcmHandle)
        return;

    snd_pcm_sframes_t ret = snd_pcm_writei(mPcmHandle, mAlsaFrame.getSoundBuffer(), mAlsaFrame.getDimension(AlsaFrame::Time).mResolution);
    if (ret == -EPIPE)
        {
        /* EPIPE xrun (underrun for playback)
           The underrun can happen when an application does not feed new samples in time to alsa-lib */
        snd_pcm_prepare(mPcmHandle);
        usleep(100000); /* FIXME: add buffering capabilities to avoid buffer underrun every second call */
        snd_pcm_writei(mPcmHandle, mAlsaFrame.getSoundBuffer(), mAlsaFrame.getDimension(AlsaFrame::Time).mResolution);
        }
    }

void AlsaDestination::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == AlsaFrame::Dimensions) &&
                (frame->getDimension(AlsaFrame::Channels).mResolution <= 2) &&
                (frame->getDimension(AlsaFrame::Time).mDelta > 0))
                {
                if (frame->getDimension(AlsaFrame::Channels).mResolution != mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution)
                    {
                    close();
                    mAlsaFrame.setChannelsNo(frame->getDimension(AlsaFrame::Channels).mResolution);
                    }
                if (frame->getDimension(AlsaFrame::Time).mDelta != mAlsaFrame.getDimension(AlsaFrame::Time).mDelta)
                    {
                    close();
                    mAlsaFrame.setSampleRate(1.0/frame->getDimension(AlsaFrame::Time).mDelta);
                    }
                if (!mPcmHandle)
                    open();

                if (mPcmHandle)
                    {
                    mAlsaFrame += *frame;
                    emit framesProcessed();
                    }
                }
            }
    }

void AlsaDestination::setDeviceName(QString aDeviceName)
    {
    mAlsaFrame.setSourceName(aDeviceName);
    emit deviceNameChanged();
    }
} // namespace media
