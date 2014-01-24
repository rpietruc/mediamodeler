#ifndef SOUNDALSADESTINATION_H
#define SOUNDALSADESTINATION_H

#include "alsaframe.h"
#include "elementbase.h"
#include <QString>
#include <alsa/asoundlib.h>

#if defined(ALSADESTINATION_LIBRARY)
#  define ALSADESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ALSADESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class ALSADESTINATIONSHARED_EXPORT SoundAlsaDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit SoundAlsaDestination(ElementFactory *aFactory, const QString &aObjectName);
    ~SoundAlsaDestination();

    ParamList getParams() const;

public slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private slots:
    void open();
    void close();
    void write();
    bool isOpened() const { return mPcmHandle != NULL; }

private:
    void process();

    snd_pcm_t *mPcmHandle;
    snd_pcm_hw_params_t *mHwParams;
    AlsaFrame mAlsaFrame;
    };

class SoundAlsaDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "AlsaDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new SoundAlsaDestination(this, aObjectName); }
    };

} // namespace media

#endif // SOUNDALSADESTINATION_H
