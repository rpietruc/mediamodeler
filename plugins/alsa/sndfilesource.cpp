#include "sndfilesource.h"
#include <QDynamicPropertyChangeEvent>

namespace media {

SndFileSource::SndFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mNextFileIndex(0),
    mSoundFile(0)
    {
    setProperty("fileList", QStringList());
    }

SndFileSource::~SndFileSource()
    {
    if (mSoundFile != 0)
        sf_close(mSoundFile);
    }

bool SndFileSource::event(QEvent *aEvent)
    {
    if (aEvent->type() == QEvent::DynamicPropertyChange)
        {
        QDynamicPropertyChangeEvent *event = (QDynamicPropertyChangeEvent*)aEvent;
        if (QString(event->propertyName().constData()) == "fileList")
            {
            if (mSoundFile != 0)
                {
                sf_close(mSoundFile);
                mSoundFile = 0;
                }
            mPathList.clear();
            foreach (QString path, property("fileList").toStringList())
                {
                QFileInfo fileInfo(path);
                mPathList.append(fileInfo.filePath());
                }
            event->accept();
            return true;
            }
        }
    return ElementBase::event(aEvent);
    }

bool SndFileSource::openNextFile()
    {
    while (mNextFileIndex < mPathList.size())
        {
        if (mSoundFile)
            {
            sf_close(mSoundFile);
            mSoundFile = 0;
            }
        SF_INFO fileInfo;
        memset(&fileInfo, 0, sizeof(fileInfo));
        mSoundFile = sf_open(qPrintable(mPathList.at(mNextFileIndex)), SFM_READ, &fileInfo);
        if (mSoundFile)
            {
            Q_ASSERT(fileInfo.samplerate);
            mAlsaFrame.setChannelsNo(fileInfo.channels);
            mAlsaFrame.setSampleTime(1.0/fileInfo.samplerate);
            mAlsaFrame.setFrameTime(AlsaFrame::DefaultFrameTime);
            mAlsaFrame.setSourceName(mPathList.at(mNextFileIndex++));
            return true;
            }
        ++mNextFileIndex;
        }
    return false;
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
    mNextFileIndex = 0;
    emit processingCompleted();
    }

} // namespace media
