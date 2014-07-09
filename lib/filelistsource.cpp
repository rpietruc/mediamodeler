#include "filelistsource.h"
#include <QDynamicPropertyChangeEvent>

namespace media {

FileListSource::FileListSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mNextFileIndex(0)
    {
    setProperty("fileList", QStringList());
    QObject::connect(this, SIGNAL(processingCompleted(bool)), this, SLOT(resetIndex(bool)));
    }

bool FileListSource::event(QEvent *aEvent)
    {
    if (aEvent->type() == QEvent::DynamicPropertyChange)
        {
        QDynamicPropertyChangeEvent *event = (QDynamicPropertyChangeEvent*)aEvent;
        if (QString(event->propertyName().constData()) == "fileList")
            {
//            if (mSoundFile != 0)
//                {
//                sf_close(mSoundFile);
//                mSoundFile = 0;
//                }
            mPathList.clear();
            foreach (QString path, property("fileList").toStringList())
                {
                QFileInfo fileInfo(path);
                mPathList.append(fileInfo.filePath());
                }
            event->accept();

            mNextFileIndex = 0;
            return true;
            }
        }
    return ElementBase::event(aEvent);
    }

} // namespace media
