#ifndef CONNECT_H
#define CONNECT_H

#include "elementbase.h"
#include <QSettings>

class QThread;

namespace media {

class ModelCreator : public QObject
    {
    Q_OBJECT

public:
    ModelCreator(QObject *aParent = NULL);
    ~ModelCreator();

    int loadFactories(const QDir& aDir);
    int createElement(const QString &aName);
    ElementBase* getElement(int aIndex);
    void deleteAllElements();
    void connectElements(int aSrcIdx, int aDstIdx);

private:
    static void connectElements(ElementBase *aSource, ElementBase *aDestination);

private:
    QVector<media::ElementBase*> mElements;
    QList<media::ElementFactory*> mFactoryList;
    QVector<QThread*> mFactoryThreads;
    };

} // namespace media

#endif // CONNECT_H
