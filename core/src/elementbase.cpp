#include "elementbase.h"
#include <QDebug>
#include <QThread>

namespace media {

ElementBase::ElementBase(QObject *aFactory, const QString &aObjectName) :
    QObject(aFactory),
    mSourceElementsNo(0),
    mRun(true)
    {
    setObjectName(aObjectName);
    }

void ElementBase::start(bool aEnabled)
    {
    mRun = aEnabled;
    if (aEnabled)
        {
        qDebug() << "start " << objectName() << ", thread " << QThread::currentThreadId();
        mCurrentReceiversReadySet.clear();
        process();
        }
    }

void ElementBase::readFrames()
    {
//    qDebug() << "readFrames " << objectName() << ", thread " << QThread::currentThreadId();
    if (!mRun)
        return;

    // get frames from sender
    Q_ASSERT(sender());
    const ElementBase* elem = qobject_cast<const ElementBase*>(sender());
    Q_ASSERT(elem);
    mSourceElementsReadySet += elem;

    // if all frames received
    Q_ASSERT(mSourceElementsNo > 0);
    Q_ASSERT(mSourceElementsNo >= mSourceElementsReadySet.size());
    if (mSourceElementsNo == mSourceElementsReadySet.size())
        {
        process();
        mSourceElementsReadySet.clear();
        }
    }

void ElementBase::processFrames()
    {
    if (!mRun)
        return;

//    qDebug() << "processFrames " << objectName() << ", thread " << QThread::currentThreadId();

    mCurrentReceiversReadySet.insert(sender());

    // if all receivers ready
    if (mConnectedReceiversSet.size() == mCurrentReceiversReadySet.size())
        {
        mCurrentReceiversReadySet.clear();
        if (mSourceElementsNo > 0)
            emit framesProcessed();
        else
            process();
        }
    }

ElementFactoryContainer::~ElementFactoryContainer()
    {
    unload();
    }

void ElementFactoryContainer::load(const QDir& aDir)
    {
    ElementFactory* factory = 0;

    QDir dir = aDir;
    //qDebug() << dir.absolutePath();
    QStringList filters;
#ifndef QT_NO_DEBUG
    filters << "*source_debug.so" << "*transform_debug.so" << "*destination_debug.so";
#else
    filters << "*destination.so" << "*transform.so" << "*destination.so";
#endif
    dir.setNameFilters(filters);

    foreach (QString fileName, dir.entryList(QDir::Files))
        {
        QPluginLoader* pluginLoader = new QPluginLoader(aDir.absoluteFilePath(fileName));
        if (pluginLoader->load())
            {
            factory = qobject_cast<ElementFactory*>(pluginLoader->instance());
            if (factory)
                mPluginLoaderList.append(pluginLoader);
            else
                {
                pluginLoader->unload();
                delete pluginLoader;
                }
            }
        else
            qDebug() << pluginLoader->errorString();
        }
    }

void ElementFactoryContainer::unload()
    {
    foreach (QPluginLoader* pluginLoader, mPluginLoaderList)
        {
        pluginLoader->unload();
        delete pluginLoader;
        }
    }

void ElementFactoryContainer::getElementFactoryList(QList<ElementFactory*> &aElementFactoryList)
    {
    foreach (QPluginLoader* pluginLoader, mPluginLoaderList)
        {
        ElementFactory* factory = qobject_cast<ElementFactory*>(pluginLoader->instance());
        Q_ASSERT(factory);
        aElementFactoryList.append(factory);
        }
    }

} // namespace media
