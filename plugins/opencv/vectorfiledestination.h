#ifndef MEDIA_VectorFileDestination_H
#define MEDIA_VectorFileDestination_H

#include "elementbase.h"
//#include <QString>
#include <QtCore/qglobal.h>

#if defined(picturefiledestination_EXPORTS)
#  define PICTUREFILEDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREFILEDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class PICTUREFILEDESTINATIONSHARED_EXPORT VectorFileDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit VectorFileDestination(ElementFactory *aFactory, const QString &aObjectName);

private:
    void process();

private:
    int mFileIndex;
    };

class VectorFileDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "VectorFileDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new VectorFileDestination(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_VectorFileDestination_H
