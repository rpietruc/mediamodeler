#include "modelbase.h"
#include <QStringList>

namespace media {

template <class Factory>
FactoryContainer<Factory>::FactoryContainer(const QDir& aDir)
{
    Factory* factory = 0;
    foreach (QString fileName, aDir.entryList(QDir::Files))
    {
        QPluginLoader* pluginLoader = new QPluginLoader(aDir.absoluteFilePath(fileName));
        if (pluginLoader->load())
        {
            factory = qobject_cast<Factory*>(pluginLoader->instance());
            if (factory)
                mPluginLoaderList.append(pluginLoader);
            else
            {
                pluginLoader->unload();
                delete pluginLoader;
            }
        }
    }
}

template <class Factory>
FactoryContainer<Factory>::~FactoryContainer()
{
    foreach (QPluginLoader* pluginLoader, mPluginLoaderList)
    {
        pluginLoader->unload();
        delete pluginLoader;
    }
}

template <class Factory>
QList<Factory*> FactoryContainer<Factory>::getFactoryList()
{
    QList<Factory*> factoryList;
    foreach (QPluginLoader* pluginLoader, mPluginLoaderList)
    {
        Factory* factory = qobject_cast<Factory*>(pluginLoader->instance());
        Q_ASSERT(factory);
        factoryList.append(factory);
    }
    return factoryList;
}

} // namespace media
