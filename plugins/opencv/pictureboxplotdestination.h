#ifndef PICTUREBOXPLOTDESTINATION_H
#define PICTUREBOXPLOTDESTINATION_H

#include "elementbase.h"
#include <QString>
#include <QtCore/qglobal.h>

#if defined(pictureboxplotdestination_EXPORTS)
#  define PICTUREBOXPLOTDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PICTUREBOXPLOTDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

class BoxPlot;

namespace media {

class PICTUREBOXPLOTDESTINATIONSHARED_EXPORT PictureBoxPlotDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit PictureBoxPlotDestination(ElementFactory *aFactory, const QString &aObjectName);
    ~PictureBoxPlotDestination();

private:
    void process();

    BoxPlot *mBoxPlot;
    };

class PictureBoxPlotDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "PictureBoxPlotDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new PictureBoxPlotDestination(this, aObjectName); }
    };

} // namespace media

#endif // PICTUREBOXPLOTDESTINATION_H
