#ifndef MEDIA_FileDestination_H
#define MEDIA_FileDestination_H

#include "elementbase.h"
#include <QString>
#include <QtCore/qglobal.h>

#if defined(filedestination_EXPORTS)
#  define FILEDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FILEDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class FILEDESTINATIONSHARED_EXPORT FileDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit FileDestination(ElementFactory *aFactory, const QString &aObjectName);

    bool event(QEvent *aEvent);

    QString getNextFilePath();

private:
    QFileInfo mFileInfo;
    int mFileIndex;
    };

} // namespace media

#endif // MEDIA_FileDestination_H
