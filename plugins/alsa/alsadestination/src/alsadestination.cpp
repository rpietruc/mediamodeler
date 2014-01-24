#include "alsadestination.h"
#include <QDebug>
#include <alsa/asoundlib.h>

namespace media {

#define PCM_OPEN_MODE 0
//#define PCM_OPEN_MODE SND_PCM_NONBLOCK

SoundAlsaDestination::SoundAlsaDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mPcmHandle(NULL)
    {
    QObject::connect(&mAlsaFrame, SIGNAL(bufferFilled()), this, SLOT(write()));

    // Allocate a hardware parameters object
    snd_pcm_hw_params_malloc(&mHwParams);

    mAlsaFrame.setSourceName("default");
    }

SoundAlsaDestination::~SoundAlsaDestination()
    {
    close();

    // Allocate a hardware parameters object
    snd_pcm_hw_params_free(mHwParams);
    }

ElementBase::ParamList SoundAlsaDestination::getParams() const
    {
    ParamList ret;
    ret["Alsa Device"] =  mAlsaFrame.getSourceName();
    return ret;
    }

void SoundAlsaDestination::setParamValue(const QString& aName, const QVariant& aValue)
    {
    Q_UNUSED(aName);
    if (mAlsaFrame.getSourceName() != aValue.toString())
        {
        close();
        mAlsaFrame.setSourceName(aValue.toString());
        }
    }

void SoundAlsaDestination::open()
    {
    close();

    // Open PCM device
    int res = snd_pcm_open(&mPcmHandle, qPrintable(mAlsaFrame.getSourceName()), SND_PCM_STREAM_PLAYBACK, PCM_OPEN_MODE);
    if (res < 0)
        qWarning() << "snd_pcm_open failed" << mAlsaFrame.getSourceName();
    else
        {
        // Fill it in with default values
        res = snd_pcm_hw_params_any(mPcmHandle, mHwParams);
        if (res < 0)
            qWarning() << "snd_pcm_hw_params_any failed";
        else
            {
            // Set the desired hardware parameters
            snd_pcm_access_t access = SND_PCM_ACCESS_RW_INTERLEAVED;
            res = snd_pcm_hw_params_set_access(mPcmHandle, mHwParams, access);
            if (res < 0)
                qWarning() << "snd_pcm_hw_params_set_access failed" << SND_PCM_ACCESS_RW_INTERLEAVED;
            else
                {
                snd_pcm_format_t val = SND_PCM_FORMAT_S16_LE;
                res = snd_pcm_hw_params_set_format(mPcmHandle, mHwParams, val);
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
            res = snd_pcm_hw_params_set_channels(mPcmHandle, mHwParams, mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution);
            if (res < 0)
                qWarning() << "channels not supported" << mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution;
            else
                {
                Q_ASSERT(mAlsaFrame.getDimension(AlsaFrame::Time).mDelta >= 0);
                unsigned int rate = 1.0/mAlsaFrame.getDimension(AlsaFrame::Time).mDelta;
                int dir = 0;
                res = snd_pcm_hw_params_set_rate(mPcmHandle, mHwParams, rate, dir);
                if (res < 0)
                    qWarning() << "sample rate not supported" << rate;
                else
                    {
                    mAlsaFrame.setFrameTime(AlsaFrame::DefaultFrameTime);
                    snd_pcm_uframes_t frameSize = mAlsaFrame.getDimension(AlsaFrame::Time).mResolution;
                    dir = 0;
                    res = snd_pcm_hw_params_set_period_size_near(mPcmHandle, mHwParams, &frameSize, &dir);
                    if (res < 0)
                        qWarning() << "frame size not supported" << frameSize;
                    else
                        {
                        // Write the parameters to the driver
                        res = snd_pcm_hw_params(mPcmHandle, mHwParams);
                        if (res < 0)
                            qWarning() << "snd_pcm_hw_params failed";
                        }
                    }
                }
            }
        }
    }

void SoundAlsaDestination::close()
    {
    if (mPcmHandle)
        {
        snd_pcm_drain(mPcmHandle);
        snd_pcm_close(mPcmHandle);
        }
    mPcmHandle = NULL;
    }

void SoundAlsaDestination::write()
    {
    Q_ASSERT(mPcmHandle);
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

void SoundAlsaDestination::process()
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
                if (!isOpened())
                    open();

                mAlsaFrame += *frame;
                emit framesProcessed();
                }
            }
    }

} // namespace media
