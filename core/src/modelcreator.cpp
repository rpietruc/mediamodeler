#include "modelcreator.h"
#include <QDebug>

namespace media {

ModelCreator::ModelCreator(QObject *aParent) :
    QObject(aParent)
    {
    }

ModelCreator::~ModelCreator()
    {
    deleteAllElements();
    }

int ModelCreator::loadFactories(const QDir& aDir)
    {
    int size = mFactoryList.size();
    ElementFactoryContainer *factoryContainer = new ElementFactoryContainer(this);
    factoryContainer->load(aDir);
    factoryContainer->getElementFactoryList(mFactoryList);
    return mFactoryList.size() - size; //new elements no
    }

int ModelCreator::createElement(const QString &aName)
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

void ModelCreator::deleteAllElements()
    {
    foreach (ElementBase* e, mElements)
        delete e;
    mElements.clear();
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
