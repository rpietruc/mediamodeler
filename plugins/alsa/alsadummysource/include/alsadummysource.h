#ifndef MEDIA_ALSADUMMYSOURCE_H
#define MEDIA_ALSADUMMYSOURCE_H

#include "elementbase.h"
#include <QStringList>
#include "alsaframe.h"
#include <QtCore/qglobal.h>

#if defined(ALSADUMMYSOURCE_LIBRARY)
#  define ALSADUMMYSOURCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define ALSADUMMYSOURCESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

struct SingleFrequencySignal;

class ALSADUMMYSOURCESHARED_EXPORT AlsaDummySource : public ElementBase
    {
    Q_OBJECT

public:
    explicit AlsaDummySource(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mAlsaFrame; }

//    ParamList getParams() const;

public slots:
//    void setParamValue(const QString& aName, const QVariant& aValue);

private:
    void setSignal(const QStringList &aSignalList);
    void process();

    AlsaFrame mAlsaFrame;
    SingleFrequencySignal* mSignal;
    };

class AlsaDummySourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "AlsaDummySource"; }
    ElementBase* createElement(const QString &aObjectName) { return new AlsaDummySource(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_ALSADUMMYSOURCE_H
