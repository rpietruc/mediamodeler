#ifndef MEDIA_PICTURERENDERDESTINATION_H
#define MEDIA_PICTURERENDERDESTINATION_H

#include "elementbase.h"
#include <QString>
#include <QtCore/qglobal.h>
#include <opencv/cv.h>
#include "pictureframes.h"

#if defined(picturerenderdestination_EXPORTS)
#  define PICTURERENDERDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTURERENDERDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

class QTimer;
class SettingsDialog;

namespace media {

class PICTURERENDERDESTINATIONSHARED_EXPORT PictureRenderDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureRenderDestination(ElementFactory *aFactory, const QString &aObjectName);
    ~PictureRenderDestination();

    bool event(QEvent *aEvent);

private slots:
    void showPicture();

private:
    void process();

private:
    QTimer *mTimer;
    IplImageFrame mPictureFrame;
    bool mImageReady;
    };

class PictureRenderDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureRenderDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureRenderDestination(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTURERENDERDESTINATION_H
