#ifndef SOUNDALSADESTINATION_H
#define SOUNDALSADESTINATION_H

#include "alsaframe.h"
#include "elementbase.h"
#include <QString>
#include <alsa/asoundlib.h>

#if defined(alsadestination_EXPORTS)
#  define ALSADESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ALSADESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class ALSADESTINATIONSHARED_EXPORT AlsaDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit AlsaDestination(ElementFactory *aFactory, const QString &aObjectName);
    ~AlsaDestination();

private slots:
    void open();
    void close();
    void write();

private:
    void process();

private:
    snd_pcm_t *mPcmHandle;
    AlsaFrame mAlsaFrame;
    };

class SoundAlsaDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "AlsaDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new AlsaDestination(this, aObjectName); }
    };

} // namespace media

#endif // SOUNDALSADESTINATION_H
