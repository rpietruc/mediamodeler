#include "sndfiledestination.h"
#include <QDebug>

namespace media {

SndFileDestination::SndFileDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mSoundFile(NULL)
    {
    QObject::connect(&mAlsaFrame, SIGNAL(bufferFilled()), this, SLOT(write()));
    }

SndFileDestination::~SndFileDestination()
    {
    close();
    }

ElementBase::ParamList SndFileDestination::getParams() const
    {
    ParamList ret;
    ret["File"] = mFileName;
    return ret;
    }

void SndFileDestination::setParamValue(const QString& aName, const QVariant& aValue)
    {
    Q_UNUSED(aName);
    if (mFileName != aValue.toString())
        {
        close();
        mFileName = aValue.toString();
        }
    }

bool SndFileDestination::open(SF_INFO &aFileInfo)
    {
    if (mSoundFile)
        close();

    mAlsaFrame.setChannelsNo(aFileInfo.channels);
    mAlsaFrame.setSampleRate(aFileInfo.samplerate);
    mAlsaFrame.setFrameTime(AlsaFrame::DefaultFrameTime);

    mSoundFile = sf_open(qPrintable(mFileName), SFM_WRITE, &aFileInfo);
    return mSoundFile;
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
                if (!mSoundFile)
                    {
                    SF_INFO fileInfo;
                    fileInfo.frames = 0;
                    fileInfo.samplerate = 1.0/frame->getDimension(AlsaFrame::Time).mDelta;
                    fileInfo.channels = frame->getDimension(AlsaFrame::Channels).mResolution;
                    fileInfo.format = SF_FORMAT_WAV|SF_FORMAT_PCM_16;
                    fileInfo.sections = 1;
                    fileInfo.seekable = 1;
                    if (!open(fileInfo))
                        qWarning() << "open file failed" << mFileName;
                    }
                mAlsaFrame += *frame;
                emit framesProcessed();
                }
            }
    }

} // namespace media
