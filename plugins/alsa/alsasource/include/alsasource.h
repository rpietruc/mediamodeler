#ifndef SOUNDALSASOURCE_H
#define SOUNDALSASOURCE_H

#include "alsaframe.h"
#include "elementbase.h"
#include <QString>
#include <alsa/asoundlib.h>

#if defined(ALSASOURCE_LIBRARY)
#  define ALSASOURCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define ALSASOURCESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class ALSASOURCESHARED_EXPORT AlsaSource : public ElementBase
    {
    Q_OBJECT
    Q_PROPERTY(QString deviceName
               READ getDeviceName
               WRITE setDeviceName
               NOTIFY deviceNameChanged)
    Q_PROPERTY(int sampleRate
               READ getSampleRate
               WRITE setSampleRate
               NOTIFY sampleRateChanged)
    Q_PROPERTY(int channelsNo
               READ getChannelsNo
               WRITE setChannelsNo
               NOTIFY channelsNoChanged)

public:
    explicit AlsaSource(ElementFactory *aFactory, const QString &aObjectName);
    ~AlsaSource();

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mAlsaFrame; }

signals:
    void deviceNameChanged();
    void sampleRateChanged();
    void channelsNoChanged();

private slots:
    void open();
    void close();

private:
    void process();

    QString getDeviceName() const { return mAlsaFrame.getSourceName(); }
    int getSampleRate() const;
    int getChannelsNo() const { return mAlsaFrame.getDimension(AlsaFrame::Channels).mResolution; }

    void setDeviceName(QString aDeviceName);
    void setSampleRate(int aSampleRate);
    void setChannelsNo(int aChannelsNo);

private:
    snd_pcm_t *mPcmHandle;
    AlsaFrame mAlsaFrame;
    };

class AlsaSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "AlsaSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new AlsaSource(this, aObjectName); }
    };

} // namespace media

#endif // SOUNDALSASOURCE_H
