#ifndef VIDEOCAMERASOURCE_H
#define VIDEOCAMERASOURCE_H

#include "elementbase.h"
#include "pictureframes.h"
#include <QString>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QtCore/qglobal.h>

#if defined(videocamerasource_EXPORTS)
#  define VIDEOCAMERASOURCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define VIDEOCAMERASOURCESHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class VIDEOCAMERASOURCESHARED_EXPORT VideoCameraSource : public ElementBase
    {
    Q_OBJECT

public:
    explicit VideoCameraSource(ElementFactory *aFactory, const QString &aObjectName);
    ~VideoCameraSource();

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mPictureFrame; }

    bool event(QEvent *aEvent);

private:
    void process();
    void open();
    int getCamIndex(const QString &aFileName);

private:
    IplImageFrame mPictureFrame;
    CvCapture* mCapture;
    int mFrameIndex;
    };

class VideoCameraSourceFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "VideoCameraSource"; }
    ElementBase* createElement(const QString &aObjectName) { return new VideoCameraSource(this, aObjectName); }
    };

} // namespace media

#endif // VIDEOCAMERASOURCE_H
