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
    Q_ASSERT(mAlsaFrame.getDimensionT(AlsaFrame::Time).mDelta);

    SF_INFO fileInfo;
    fileInfo.frames = 0;
    fileInfo.samplerate = 1.0/mAlsaFrame.getDimensionT(AlsaFrame::Time).mDelta;
    fileInfo.channels = mAlsaFrame.getDimensionT(AlsaFrame::Channels).mResolution;
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
    sf_writef_short(mSoundFile, mAlsaFrame.getSoundBuffer(), mAlsaFrame.getDimensionT(AlsaFrame::Time).mResolution);
    }

void SndFileDestination::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (frame->getMaxDimension() == AlsaFrame::Dimensions)
                {
                Q_ASSERT(frame->getDimensionT(AlsaFrame::Time).mDelta > 0);
                if (mAlsaFrame.getSourceName() != property("fileName").toString())
                    {
                    close();
                    mAlsaFrame.setSourceName(property("fileName").toString());
                    }
                if (mAlsaFrame.getDimensionT(AlsaFrame::Time).mDelta != frame->getDimensionT(AlsaFrame::Time).mDelta)
                    {
                    close();
                    mAlsaFrame.setSampleTime(frame->getDimensionT(AlsaFrame::Time).mDelta);
                    }
                if (mAlsaFrame.getDimensionT(AlsaFrame::Channels).mResolution != frame->getDimensionT(AlsaFrame::Channels).mResolution)
                    {
                    close();
                    mAlsaFrame.setChannelsNo(frame->getDimensionT(AlsaFrame::Channels).mResolution);
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
