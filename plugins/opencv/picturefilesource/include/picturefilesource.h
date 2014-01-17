#ifndef MEDIA_PICTUREFILESOURCE_H
#define MEDIA_PICTUREFILESOURCE_H

#include "elementbase.h"
#include "pictureframes.h"
#include <QStringList>
#include <QtCore/qglobal.h>

#if defined(PICTUREFILESOURCE_LIBRARY)
#  define PICTUREFILESOURCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREFILESOURCESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREFILESOURCESHARED_EXPORT PictureFileSource : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureFileSource(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

    ParamList getParams();

public slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private:
    void process();

    IplImageFrame mPictureFrame;
    QStringList mPathList;
    int mNextFileIndex;
    };

class PictureFileSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureFileSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureFileSource(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTUREFILESOURCE_H
