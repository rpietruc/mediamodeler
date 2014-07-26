#include "propertiesbox.h"
#include "guielementlist.h"
#include "fileguielement.h"
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QTime>
#include <QTimer>
#include <QDebug>

using namespace media;

PropertiesBox::PropertiesBox(QObject *aPropertiesObject, QWidget *aParent) :
    QGroupBox(aPropertiesObject->objectName(), aParent),
    mPropertiesObject(aPropertiesObject),
    mTextBrowser(new QTextBrowser(this)),
    mTimeStamper(new QTimer(this))
    {
    QList<QByteArray> list = aPropertiesObject->dynamicPropertyNames();
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    if (list.count() > 0)
        {
        GuiElementBase* gui = NULL;
        if (QString(list.first().constData()) == "fileName")
            {
            if (list.count() == 1)
                gui = new FileGuiElement(this);
            else
                qWarning() << list.first().constData() << " property name reserved for single property box";
            }
        else if (QString(list.first().constData()) == "fileList")
            {
            if (list.count() == 1)
                gui = new FilesGuiElement(this);
            else
                qWarning() << list.first().constData() << " property name reserved for single property box";
            }
        if (!gui)
            gui = new GuiElementList(this);

        foreach (QByteArray name, list)
            gui->addProperty(QString(name.constData()), aPropertiesObject->property(name.constData()));

        QObject::connect(gui, SIGNAL(paramChanged(QString, QVariant)), this, SLOT(paramChanged(QString, QVariant)));
        verticalLayout->addWidget(gui);
        }
    verticalLayout->addWidget(mTextBrowser);
    mTimeStamper->setInterval(25);
    QObject::connect(mTimeStamper, SIGNAL(timeout()), this, SLOT(logTimestamp()));
    }

void PropertiesBox::logMessage(int aPriority, const QString &aInfo)
    {
    log(QColor(static_cast<Qt::GlobalColor>(aPriority)), aInfo);

    if (!mTimeStamper->isActive())
        {
        logTimestamp(); //likely there was no stamp for a while
        mTimeStamper->start();
        }
    mCountDown = 3;
    }

void PropertiesBox::logTimestamp()
    {
    QTime time = QTime::currentTime();
    log(QColor(Qt::gray), QString("time: %1:%2:%3.%4").arg(time.hour()).arg(time.minute()).arg(time.second()).arg(time.msec(), 3, 10, QLatin1Char('0')));

    if (0 >= --mCountDown)
        mTimeStamper->stop();
    }

void PropertiesBox::log(const QColor &aColor, const QString &aInfo)
    {
    mTextBrowser->setTextColor(aColor);
    mTextBrowser->insertPlainText(aInfo + "\n");
    mTextBrowser->ensureCursorVisible();
    }

void PropertiesBox::paramChanged(const QString &aName, const QVariant &aValue)
    {
    mPropertiesObject->setProperty(qPrintable(aName), aValue);
    emit settingChanged(title(), aName, aValue);
    }
