#ifndef SPECTROGRAMDESTINATION_H
#define SPECTROGRAMDESTINATION_H

#include "elementbase.h"
#include "spectrumframe.h"
#include <QtCore/qglobal.h>

#if defined(SPECTROGRAMDESTINATION_EXPORTS)
#  define SPECTROGRAMDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPECTROGRAMDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

class SpectrogramWindow;

namespace media {

class SPECTROGRAMDESTINATIONSHARED_EXPORT SpectrogramDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit SpectrogramDestination(ElementFactory *aFactory, const QString &aObjectName);
    ~SpectrogramDestination();

private:
    void process();

signals:
    void dataUpdated();

private:
    SpectrogramWindow *mSpectrogram;
    SpectrumFrame mSpectrumFrame;
    };

class SpectrogramDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "SpectrogramDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new SpectrogramDestination(this, aObjectName); }
    };

} // namespace media

#endif // SPECTROGRAMDESTINATION_H
