#ifndef ELEMENTBASE_H
#define ELEMENTBASE_H

#include "globaldefs.h"
#include <QObject>
#include <QList>
#include "framebase.h"
#include <QDir>
#include <QString>
#include <QPluginLoader>
#include <QMap>
#include <QVariant>

class SettingsDialog;

namespace media {

//! ElementBase
/*!
    @details

     <PRE>
       -----------------------          -------------------------------------------------          ---------------------------
      | SIGNAL(framesReady()) | -----> | SLOT(readFrames()) -----> SIGNAL(framesReady()) | -----> | SLOT(readFrames())        |
      | SLOT(processFrames()) | <----- | SIGNAL(framesProcessed()) SLOT(processFrames()) | <----- | SIGNAL(framesProcessed()) |
       -----------------------          -------------------------------------------------          ---------------------------

    </PRE>
*/

class MEDIAMODELSHARED_EXPORT ElementBase : public QObject
    {
    Q_OBJECT

public:
    ElementBase(QObject *aFactory, const QString &aObjectName);

    virtual int getFramesNo() const { return 0; }
    virtual const FrameBase *getFrame(int) const { return 0; }

    void insertReceiver(QObject *aReceiver) { mConnectedReceiversSet.insert(aReceiver); }

signals:
    void framesProcessed();
    void framesReady();
    void processingCompleted(bool aRunning = false);
    void logMessage(int aPriority, const QString &aText);

public slots:
    void incrementSourceElementsNo() { ++mSourceElementsNo; }

    virtual void processFrames();
    void setRunning(bool aEnable);
    void readFrames();

protected:
    virtual void process() = 0;

protected:
    QSet<const ElementBase*> mSourceElementsReadySet;

private:
    int mSourceElementsNo;

    QSet<QObject*> mConnectedReceiversSet;
    QSet<QObject*> mCurrentReceiversReadySet;

    bool mRunning;
    };

class MEDIAMODELSHARED_EXPORT ElementFactory : public QObject
    {
    Q_OBJECT

public:
    virtual const char* getElementName() const = 0;
    virtual ElementBase* createElement(const QString &aObjectName) = 0;
    };

class ElementFactoryContainer : public QObject
    {
    Q_OBJECT

public:
    inline ElementFactoryContainer(QObject *aParent = NULL) : QObject(aParent) {}
    virtual ~ElementFactoryContainer();
    void getElementFactoryList(QList<ElementFactory*> &aElementFactoryList);
    void load(const QDir& aDir);
    void unload();

private:
    QList<QPluginLoader*> mPluginLoaderList;
    };

} // namespace media

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(media::ElementFactory, "media/elementfactoryinterface/1.0/1.0");
QT_END_NAMESPACE

#endif // ELEMENTBASE_H
