#include "filedestination.h"
#include <QDynamicPropertyChangeEvent>

namespace media {

FileDestination::FileDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mFileIndex(0)
    {
    setProperty("fileName", "output");
    }

bool FileDestination::event(QEvent *aEvent)
    {
    if (aEvent->type() == QEvent::DynamicPropertyChange)
        {
        QDynamicPropertyChangeEvent *event = (QDynamicPropertyChangeEvent*)aEvent;
        if (QString(event->propertyName().constData()) == "fileName")
            {
            mFileInfo.setFile(property("fileName").toString());
            event->accept();
            mFileIndex = 0;
            return true;
            }
        }
    return ElementBase::event(aEvent);
    }

QString FileDestination::getNextFilePath()
    {
    QString fileName = mFileInfo.fileName();
    if (mFileIndex > 0)
        {
        QString suffix = mFileInfo.completeSuffix();
        if (!suffix.isEmpty())
            fileName.replace(fileName.lastIndexOf(suffix), suffix.size(), QString("%1.").arg(mFileIndex) + suffix);
        else
            fileName += QString(".%1").arg(mFileIndex);
        }
    ++mFileIndex;
    return mFileInfo.dir().path() + QDir::separator() + fileName;
    }

} // namespace media
