#include "propertiesbox.h"
#include "guielementlist.h"
#include "fileguielement.h"
#include <QVBoxLayout>
#include <QMetaProperty>

using namespace media;

PropertiesBox::PropertiesBox(QObject *aPropertiesObject, QWidget *aParent) :
    QGroupBox(aPropertiesObject->objectName(), aParent),
    mPropertiesObject(aPropertiesObject)
    {
    const QMetaObject *metaobject = aPropertiesObject->metaObject();
    int count = metaobject->propertyCount();
    if (count > 1) //first property is object name
        {
        GuiElementBase* gui = NULL;
        if (count == 2)
            {
            QMetaProperty metaproperty = metaobject->property(1);
            QString name = metaproperty.name();
            if (name == "fileName")
                gui = new FileGuiElement(this);
            else if (name == "fileList")
                gui = new FilesGuiElement(this);
            }
        if (!gui)
            gui = new GuiElementList(this);

        for (int i = 1; i < count; ++i)
            {
            QMetaProperty metaproperty = metaobject->property(i);
            const char *name = metaproperty.name();
            QVariant value = aPropertiesObject->property(name);
            gui->addProperty(name, value);
            }
        QObject::connect(gui, SIGNAL(paramChanged(QString, QVariant)), this, SLOT(paramChanged(QString, QVariant)));
        QVBoxLayout *verticalLayout = new QVBoxLayout(this);
        verticalLayout->addWidget(gui);
        }
    }

void PropertiesBox::paramChanged(const QString &aName, const QVariant &aValue)
    {
    mPropertiesObject->setProperty(qPrintable(aName), aValue);
    emit settingChanged(title(), aName, aValue);
    }
