#ifndef MEDIA_MatrixFileDestination_H
#define MEDIA_MatrixFileDestination_H

#include "elementbase.h"
//#include <QString>
#include <QtCore/qglobal.h>

#if defined(matrixfiledestination_EXPORTS)
#  define MATRIXFILEDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MATRIXFILEDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class MATRIXFILEDESTINATIONSHARED_EXPORT MatrixFileDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit MatrixFileDestination(ElementFactory *aFactory, const QString &aObjectName);

private:
    void process();
    };

class MatrixFileDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "MatrixFileDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new MatrixFileDestination(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_MatrixFileDestination_H
