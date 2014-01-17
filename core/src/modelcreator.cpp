#include "modelcreator.h"
#include <QDebug>

namespace media {

ModelCreator::ModelCreator(const QDir& aDir, QObject *aParent) :
    QObject(aParent)
    {
    ElementFactoryContainer *factoryContainer = new ElementFactoryContainer(this);
    factoryContainer->load(aDir);
    factoryContainer->getElementFactoryList(mFactoryList);
    }

ModelCreator::~ModelCreator()
    {
    clearModel();
    }

void ModelCreator::clearModel()
    {
    foreach (ElementBase* e, mElements)
        delete e;
    mElements.clear();
    }

int ModelCreator::addElement(const QString &aName)
    {
    int index = -1;
    foreach (ElementFactory* factory, mFactoryList)
        if (aName == factory->getElementName())
            {
            index = mElements.size();
            mElements.push_back(factory->createElement(aName));
            mElements[index]->setObjectName(QString("%1%2").arg(aName).arg(index));
            break;
            }
    return index;
    }

ElementBase* ModelCreator::getElement(int aIndex)
    {
    if ((aIndex < mElements.size()) && (aIndex >= 0))
        return mElements[aIndex];
    //else
    return NULL;
    }

void ModelCreator::connectElements(int aSrcIdx, int aDstIdx)
    {
    Q_ASSERT((aDstIdx >= 0) && (aSrcIdx >= 0) && (aDstIdx < mElements.size()) && (aSrcIdx < mElements.size()));
    connectElements(mElements[aSrcIdx], mElements[aDstIdx]);
    }

void ModelCreator::connectElements(ElementBase *aSource, ElementBase *aDestination)
    {
    QObject::connect(aDestination, SIGNAL(framesProcessed()), aSource, SLOT(processFrames()), Qt::QueuedConnection);
    QObject::connect(aSource, SIGNAL(framesReady()), aDestination, SLOT(readFrames()), Qt::QueuedConnection);

    aSource->insertReceiver(aDestination);
    aDestination->incrementSourceElementsNo();

    qDebug() << aSource->objectName() << " connected to " << aDestination->objectName();
    }

} // namespace media
