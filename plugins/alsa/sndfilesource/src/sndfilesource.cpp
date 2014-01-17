#include "sndfilesource.h"

namespace media {

SndFileSource::SndFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mNextFileIndex(0),
    mSoundFile(0)
    {
    }

SndFileSource::~SndFileSource()
    {
    if (mSoundFile != 0)
        sf_close(mSoundFile);
    }

ElementBase::ParamList SndFileSource::getParams()
    {
    ParamList ret;
    ret["Files"] =  mPathList;
    return ret;
    }

void SndFileSource::setParamValue(const QString& aName, const QVariant& aValue)
    {
    Q_UNUSED(aName);
    QStringList pathList = aValue.toStringList();
    if (mSoundFile != 0)
        {
        sf_close(mSoundFile);
        mSoundFile = 0;
        }
    mPathList.clear();
    foreach (QString path, pathList)
        {
        QFileInfo fileInfo(path);
        mPathList.append(fileInfo.filePath());
        }
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
            mAlsaFrame.setChannelsNo(fileInfo.channels);
            mAlsaFrame.setSampleRate(fileInfo.samplerate);
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
            sf_count_t res = sf_readf_short(mSoundFile, mAlsaFrame.getSoundBuffer(), mAlsaFrame.getDimension(AlsaFrame::Time).mResolution);
            if (res > 0)
                {
                // keep constant frame size, populate with zeros
                if (res < mAlsaFrame.getDimension(AlsaFrame::Time).mResolution)
                    memset(mAlsaFrame.getSoundBuffer() + res*mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution, 0, (mAlsaFrame.getDimension(AlsaFrame::Time).mResolution - res)*sizeof(qint16)*mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution);

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
