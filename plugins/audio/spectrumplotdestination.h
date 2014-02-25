#ifndef MEDIA_SOUNDPLOTDESTINATION_H
#define MEDIA_SOUNDPLOTDESTINATION_H

#include "elementbase.h"
#include <QtCore/qglobal.h>

#if defined(SOUNDPLOTDESTINATION_EXPORTS)
#  define SOUNDPLOTDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SOUNDPLOTDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

class PlotDialog;

namespace media {

class SOUNDPLOTDESTINATIONSHARED_EXPORT SpectrumPlotDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit SpectrumPlotDestination(ElementFactory *aFactory, const QString &aObjectName);
    ~SpectrumPlotDestination();

signals:
    void sampleReceived(double aKey, double aValue, int aIndex);

private slots:
    void open(const QString &aSettings);

private:
    void process();

    PlotDialog *mPlot;
    };

class SpectrumPlotDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "SpectrumPlotDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new SpectrumPlotDestination(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_SOUNDPLOTDESTINATION_H
