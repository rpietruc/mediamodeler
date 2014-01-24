#include "elementbox.h"
#include "guielementlist.h"
#include "fileguielement.h"
#include <QVBoxLayout>

using namespace media;

ElementBox::ElementBox(const ElementBase* aElement, QWidget *aParent) :
    QGroupBox(aElement->objectName(), aParent)
    {
    ElementBase::ParamList list = aElement->getParams();
    if (!list.isEmpty())
        {
        GuiElementBase* gui = NULL;
        if (list.size() == 1)
            {
            if (list.find("File") != list.end())
                gui = new FileGuiElement(this);
            else if (list.find("Files") != list.end())
                gui = new FilesGuiElement(this);
            }
        if (!gui)
            gui = new GuiElementList(this);

        gui->init(list);
        QObject::connect(gui, SIGNAL(paramChanged(QString, QVariant)), this, SLOT(paramChanged(QString, QVariant)));
        QObject::connect(gui, SIGNAL(paramChanged(QString, QVariant)), aElement, SLOT(setParamValue(QString, QVariant)));

        QVBoxLayout *verticalLayout = new QVBoxLayout(this);
        verticalLayout->addWidget(gui);
        }
    }
