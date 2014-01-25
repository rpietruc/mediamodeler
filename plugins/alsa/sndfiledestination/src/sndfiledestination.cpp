#include "sndfiledestination.h"
#include <QDebug>

namespace media {

SndFileDestination::SndFileDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mSoundFile(NULL)
    {
    setProperty("fileName", "output.wav");
    QObject::connect(&mAlsaFrame, SIGNAL(bufferFilled()), this, SLOT(write()));
    }

SndFileDestination::~SndFileDestination()
    {
    close();
    }

void SndFileDestination::open()
    {
    Q_ASSERT(mAlsaFrame.getDimension(AlsaFrame::Time).mDelta);

    SF_INFO fileInfo;
    fileInfo.frames = 0;
    fileInfo.samplerate = 1.0/mAlsaFrame.getDimension(AlsaFrame::Time).mDelta;
    fileInfo.channels = mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution;
    fileInfo.format = SF_FORMAT_WAV|SF_FORMAT_PCM_16;
    fileInfo.sections = 1;
    fileInfo.seekable = 1;

    Q_ASSERT(!mSoundFile);
    mSoundFile = sf_open(qPrintable(mAlsaFrame.getSourceName()), SFM_WRITE, &fileInfo);
    if (!mSoundFile)
        qWarning() << "open file failed" << mAlsaFrame.getSourceName();
    }

void SndFileDestination::close()
    {
    if (mSoundFile)
        {
        sf_write_sync(mSoundFile);
        sf_close(mSoundFile);
        }
    mSoundFile = NULL;
    }

void SndFileDestination::write()
    {
    Q_ASSERT(mSoundFile);
    sf_writef_short(mSoundFile, mAlsaFrame.getSoundBuffer(), mAlsaFrame.getDimension(AlsaFrame::Time).mResolution);
    }

void SndFileDestination::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (frame->getMaxDimension() == AlsaFrame::Dimensions)
                {
                Q_ASSERT(frame->getDimension(AlsaFrame::Time).mDelta > 0);
                if (mAlsaFrame.getSourceName() != property("fileName").toString())
                    {
                    close();
                    mAlsaFrame.setSourceName(property("fileName").toString());
                    }
                if (mAlsaFrame.getDimension(AlsaFrame::Time).mDelta != frame->getDimension(AlsaFrame::Time).mDelta)
                    {
                    close();
                    mAlsaFrame.setSampleTime(frame->getDimension(AlsaFrame::Time).mDelta);
                    }
                if (mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution != frame->getDimension(AlsaFrame::Channels).mResolution)
                    {
                    close();
                    mAlsaFrame.setChannelsNo(frame->getDimension(AlsaFrame::Channels).mResolution);
                    }

                if (!mSoundFile)
                    {
                    mAlsaFrame.setFrameTime(AlsaFrame::DefaultFrameTime);
                    open();
                    }
                if (mSoundFile)
                    {
                    mAlsaFrame += *frame;
                    emit framesProcessed();
                    }
                }
            }
    }

} // namespace media
