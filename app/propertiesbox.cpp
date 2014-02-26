#include "propertiesbox.h"
#include "guielementlist.h"
#include "fileguielement.h"
#include <QVBoxLayout>
#include <QTextBrowser>

using namespace media;

PropertiesBox::PropertiesBox(QObject *aPropertiesObject, QWidget *aParent) :
    QGroupBox(aPropertiesObject->objectName(), aParent),
    mPropertiesObject(aPropertiesObject),
    mTextBrowser(new QTextBrowser(this))
    {
    QList<QByteArray> list = aPropertiesObject->dynamicPropertyNames();
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    if (list.count() > 0)
        {
        GuiElementBase* gui = NULL;
        if (list.count() == 1)
            {
            if (QString(list.first().constData()) == "fileName")
                gui = new FileGuiElement(this);
            else if (QString(list.first().constData()) == "fileList")
                gui = new FilesGuiElement(this);
            }
        if (!gui)
            gui = new GuiElementList(this);

        foreach (QByteArray name, list)
            gui->addProperty(QString(name.constData()), aPropertiesObject->property(name.constData()));

        QObject::connect(gui, SIGNAL(paramChanged(QString, QVariant)), this, SLOT(paramChanged(QString, QVariant)));
        verticalLayout->addWidget(gui);
        }
    verticalLayout->addWidget(mTextBrowser);
    }

void PropertiesBox::logMessage(int aPriority, const QString &aInfo)
    {
    mTextBrowser->setTextColor(QColor(static_cast<Qt::GlobalColor>(aPriority)));
    mTextBrowser->insertPlainText(aInfo + "\n");
    mTextBrowser->ensureCursorVisible();
    }

void PropertiesBox::paramChanged(const QString &aName, const QVariant &aValue)
    {
    mPropertiesObject->setProperty(qPrintable(aName), aValue);
    emit settingChanged(title(), aName, aValue);
    }
