#ifndef MEDIA_PICTURERENDERDESTINATION_H
#define MEDIA_PICTURERENDERDESTINATION_H

#include "elementbase.h"
#include <QString>
#include <QtCore/qglobal.h>
#include <opencv/cv.h>
#include "pictureframes.h"

#if defined(PICTURERENDERDESTINATION_LIBRARY)
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

    ParamList getParams() const;

protected slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private slots:
    void showPicture();

private:
    void process();

    QTimer *mTimer;
    IplImageFrame mPictureFrame;
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
