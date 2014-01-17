#include "guielementlist.h"
#include "ui_guielementlist.h"
#include <QLabel>
#include <QLineEdit>

namespace media {

GuiElementList::GuiElementList(QWidget *aParent) :
    GuiElementBase(aParent),
    mUi(new Ui::GuiElementList)
    {
    mUi->setupUi(this);
    }

GuiElementList::~GuiElementList()
    {
    delete mUi;
    }

void GuiElementList::init(const ElementBase::ParamList& aParams)
    {
    const QList<QString> keys = aParams.keys();
    int i = 0;
    foreach (QString key, keys)
        {
        QLabel *label = new QLabel(this);
        mUi->formLayout->setWidget(i, QFormLayout::LabelRole, label);
        label->setText(key);

        QLineEdit* value = new QLineEdit(this);
        mUi->formLayout->setWidget(i, QFormLayout::FieldRole, value);
        value->setText(aParams[key].toString());
        QObject::connect(value, SIGNAL(textChanged(QString)), this, SLOT(notifySettingsChanged(QString)));

        ++i;
        }
    }

void GuiElementList::notifySettingsChanged(const QString& aText)
    {
    QWidget* widget = qobject_cast<QWidget*>(sender());
    Q_ASSERT(widget);

    for (int i = 0; i < mUi->formLayout->count(); ++i)
        if (mUi->formLayout->itemAt(i, QFormLayout::FieldRole)->widget() == widget)
            {
            QLabel* label = qobject_cast<QLabel*>(mUi->formLayout->itemAt(i, QFormLayout::LabelRole)->widget());
            emit paramChanged(label->text(), QVariant(aText));
            break;
            }
    }

} // namespace media
