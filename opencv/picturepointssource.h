#ifndef MEDIA_PICTUREPOINTSSOURCE_H
#define MEDIA_PICTUREPOINTSSOURCE_H

#include "sourcebase.h"
#include "pictureframes.h"
#include <QStringList>
#include <QFileDialog>
#include <QtCore/qglobal.h>

#if defined(picturepointssource_LIBRARY)
#  define PICTUREPOINTSSOURCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREPOINTSSOURCESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREPOINTSSOURCESHARED_EXPORT PicturePointsSource : public SourceBase
{
    Q_OBJECT

public:
    PicturePointsSource(SourceFactory *aFactory, const QString &aObjectName);
    ~PicturePointsSource();

    virtual const FrameBase *getFrame() const { return &mPointsFrame; }

public slots:
    virtual void initSettings(const QString& aSettings);
    virtual void showSettingsDialog();

private slots:
    void open(const QStringList &aPathList);

private:
    virtual bool read();

    PointsFrame mPointsFrame;
    QStringList mPathList;
    int mNextFileIndex;
    QFileDialog mFileDialog;
    const QString mPathSeparator;

};

class PicturePointsSourceFactory : public SourceFactory
{
    Q_OBJECT
    Q_INTERFACES(media::SourceFactory)

protected:
    inline const char* getSourceName() const { return "PicturePointsSource"; }
    inline SourceBase* createSource(const QString &aObjectName) { return new PicturePointsSource(this, aObjectName); }

};

} // namespace media

#endif // MEDIA_PICTUREPOINTSSOURCE_H
