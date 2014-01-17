#ifndef MEDIA_PICTUDRAWDESTINATION_H
#define MEDIA_PICTUDRAWDESTINATION_H

#include "elementbase.h"
#include <QString>
#include <QtCore/qglobal.h>

#if defined(PICTUDRAWDESTINATION_LIBRARY)
#  define PICTUDRAWDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUDRAWDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

class QTimer;
class DrawWidget;
class QImage;

namespace media {

class PICTUDRAWDESTINATIONSHARED_EXPORT PictureDrawDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureDrawDestination(ElementFactory *aFactory, const QString &aObjectName);
    ~PictureDrawDestination();

    ParamList getParams();

protected slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private slots:
    void showPicture();

private:
    void process();

    QTimer *mTimer;
    DrawWidget *mWindow;
    QImage *mImage;
    };

class PictureDrawDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureDrawDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureDrawDestination(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PICTUDRAWDESTINATION_H