#ifndef MEDIA_PotraceFileDestination_H
#define MEDIA_PotraceFileDestination_H

#include "elementbase.h"
//#include <QString>
#include <QtCore/qglobal.h>

#if defined(potracefiledestination_EXPORTS)
#  define POTRACEFILEDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define POTRACEFILEDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class POTRACEFILEDESTINATIONSHARED_EXPORT PotraceFileDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit PotraceFileDestination(ElementFactory *aFactory, const QString &aObjectName);

private:
    void process();
    };

class PotraceFileDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PotraceFileDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new PotraceFileDestination(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_PotraceFileDestination_H
