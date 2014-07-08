#ifndef MEDIA_FILELISTSOURCE_H
#define MEDIA_FILELISTSOURCE_H

#include "elementbase.h"
#include <QStringList>
#include <QtCore/qglobal.h>

#if defined(filelistsource_EXPORTS)
#  define FILELISTSOURCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define FILELISTSOURCESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class FILELISTSOURCESHARED_EXPORT FileListSource : public ElementBase
    {
    Q_OBJECT

public:
    explicit FileListSource(ElementFactory *aFactory, const QString &aObjectName);

    bool event(QEvent *aEvent);

    QString getNextFilePath()
        {
        if ((mNextFileIndex < 0) || (mNextFileIndex >= mPathList.size()))
            return "";
        //else
        return mPathList.at(mNextFileIndex++);
        }

private:
    QStringList mPathList;
    int mNextFileIndex;
    };

} // namespace media

#endif // MEDIA_FILELISTSOURCE_H
