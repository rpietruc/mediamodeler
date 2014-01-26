#ifndef MEDIA_MODELBASE_H
#define MEDIA_MODELBASE_H

#include <QObject>
#include <QList>
#include <QPluginLoader>
#include <QDir>
#include <QString>

namespace media {

class MEDIAMODELSHARED_EXPORT ModelFactory : public QObject
{
    Q_OBJECT

public:
    virtual const char* getModelName() const = 0;
    virtual ModelBase* createModel() = 0;

};



} // namespace media

#endif // MEDIA_MODELBASE_H
