#ifndef SOUNDDEVICESOURCE_H
#define SOUNDDEVICESOURCE_H

#include "soundframe.h"
#include "elementbase.h"
#include <QAudio>

#if defined(SOUNDDEVICESOURCE_LIBRARY)
#  define SOUNDDEVICESOURCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define SOUNDDEVICESOURCESHARED_EXPORT Q_DECL_IMPORT
#endif

class QAudioInput;
class SettingsDialog;

namespace media {

class SOUNDDEVICESOURCESHARED_EXPORT SoundDeviceSource : public ElementBase
    {
    Q_OBJECT

public:
    explicit SoundDeviceSource(ElementFactory *aFactory, const QString &aObjectName);
    ~SoundDeviceSource();

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mSoundFrame; }

private slots:
    void notified();
    void stateChanged(QAudio::State state);
    void readMore();

private:
    void process();
    void open();
    void close();

private:
    QAudioInput* mAudioInput;
    QIODevice* mInputDevice;
    SoundFrame mSoundFrame;
    };

class SoundDeviceSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "SoundDeviceSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new SoundDeviceSource(this, aObjectName); }
    };

} // namespace media

#endif // SOUNDDEVICESOURCE_H
