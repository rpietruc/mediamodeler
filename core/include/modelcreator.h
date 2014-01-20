#ifndef CONNECT_H
#define CONNECT_H

#include "elementbase.h"
#include <QSettings>

namespace media {

class ModelCreator : public QObject
    {
    Q_OBJECT

public:
    ModelCreator(QObject *aParent = NULL);
    ~ModelCreator();

    int loadElements(const QDir& aDir);
    void clearModel();

    int addElement(const QString &aName);

    ElementBase* getElement(int aIndex);
    void connectElements(int aSrcIdx, int aDstIdx);

private:
    static void connectElements(ElementBase *aSource, ElementBase *aDestination);

private:
    QVector<media::ElementBase*> mElements;
    QList<media::ElementFactory*> mFactoryList;
    };

} // namespace media

#endif // CONNECT_H
