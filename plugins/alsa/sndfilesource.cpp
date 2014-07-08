#include "sndfilesource.h"
#include <QDynamicPropertyChangeEvent>

namespace media {

SndFileSource::SndFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    FileListSource(aFactory, aObjectName),
    mSoundFile(0)
    {
    }

SndFileSource::~SndFileSource()
    {
    if (mSoundFile != 0)
        sf_close(mSoundFile);
    }

bool SndFileSource::openNextFile()
    {
    if (mSoundFile)
        {
        sf_close(mSoundFile);
        mSoundFile = 0;
        }

    QString filename = getNextFilePath();
    if (filename.isEmpty())
        return false;

    SF_INFO fileInfo;
    memset(&fileInfo, 0, sizeof(fileInfo));
    mSoundFile = sf_open(qPrintable(filename), SFM_READ, &fileInfo);
    if (!mSoundFile)
        return false;

    Q_ASSERT(fileInfo.samplerate);
    mAlsaFrame.setChannelsNo(fileInfo.channels);
    mAlsaFrame.setSampleTime(1.0/fileInfo.samplerate);
    mAlsaFrame.setFrameTime(AlsaFrame::DefaultFrameTime);
    mAlsaFrame.setSourceName(filename);
    return true;
    }

void SndFileSource::process()
    {
    //open first file
    if (mSoundFile || openNextFile())
        {
        // read audio data
        do
            {
            sf_count_t res = sf_readf_short(mSoundFile, mAlsaFrame.getSoundBuffer(), mAlsaFrame.getDimensionT(AlsaFrame::Time).mResolution);
            if (res > 0)
                {
                // keep constant frame size, populate with zeros
                if (res < mAlsaFrame.getDimensionT(AlsaFrame::Time).mResolution)
                    memset(mAlsaFrame.getSoundBuffer() + res*mAlsaFrame.getDimensionT(AlsaFrame::Channels).mResolution, 0, (mAlsaFrame.getDimensionT(AlsaFrame::Time).mResolution - res)*sizeof(qint16)*mAlsaFrame.getDimensionT(AlsaFrame::Channels).mResolution);

                mAlsaFrame.incrementTimeStamp();
                emit framesReady();
                return;
                }
            }
        //open next file
        while (openNextFile());
        }

    //error openning file
    emit processingCompleted();
    }

} // namespace media
