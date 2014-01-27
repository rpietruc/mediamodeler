#include "picturepointssource.h"
#include <QtPlugin>

namespace media {

PicturePointsSource::PicturePointsSource(SourceFactory *aFactory, const QString &aObjectName) :
    SourceBase(aFactory, aObjectName),
    mNextFileIndex(0),
    mPathSeparator(":")
{
    mFileDialog.setFileMode(QFileDialog::ExistingFiles);
    QObject::connect(&mFileDialog, SIGNAL(filesSelected(QStringList)), this, SLOT(open(QStringList)));
}

PicturePointsSource::~PicturePointsSource()
{
}

void PicturePointsSource::open(const QStringList &aPathList)
{
    mPathList.clear();
    foreach (QString path, aPathList)
    {
        QFileInfo fileInfo(path);
        mPathList.append(fileInfo.filePath());
    }
    emit settingsChanged(mPathList.join(mPathSeparator));
}

bool PicturePointsSource::read()
{
    while (mNextFileIndex < mPathList.size())
    {
        if (mPointsFrame.readFromFile(mPathList.at(mNextFileIndex++)))
            return true;
    }
    mNextFileIndex = 0;
    return false;
}

void PicturePointsSource::showSettingsDialog()
{
    mFileDialog.show();
}

void PicturePointsSource::initSettings(const QString& aSettings)
{
    mPathList = aSettings.split(mPathSeparator);
}

Q_EXPORT_PLUGIN2(PLUGINTARGETNAME, PicturePointsSourceFactory);

} // namespace media
