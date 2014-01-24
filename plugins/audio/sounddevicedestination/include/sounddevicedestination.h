#ifndef SOUNDDEVICEDESTINATION_H
#define SOUNDDEVICEDESTINATION_H

#include "audiooutputdevice.h"
#include "elementbase.h"
#include <QAudio>
#include <QAudioDeviceInfo>

#if defined(SOUNDDEVICEDESTINATION_LIBRARY)
#  define SOUNDDEVICEDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SOUNDDEVICEDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

class QAudioOutput;

namespace media {

class SOUNDDEVICEDESTINATIONSHARED_EXPORT SoundDeviceDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit SoundDeviceDestination(ElementFactory *aFactory, const QString &aObjectName);
    ~SoundDeviceDestination();

    ParamList getParams() const;

public slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private slots:
    void notified();
    void stateChanged(QAudio::State aState);
    void checkBytesRead(int aBytesRead);

private:
    void process();
    void setDeviceInfo(const QString &aDeviceName);
    void open();
    void close();
    bool isOpened() const { return mAudioOutput != NULL; }

private:
    QAudioOutput* mAudioOutput;
    AudioOutputDevice mAudioOutputDevice;
    qreal mLastFrameTime;
    bool mWaitForBytesRead;
    };

class SoundDeviceDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "SoundDeviceDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new SoundDeviceDestination(this, aObjectName); }
    };

} // namespace media

#endif // SOUNDDEVICEDESTINATION_H
