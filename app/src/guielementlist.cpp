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

void GuiElementList::addProperty(const QString &aName, const QVariant &aValue)
    {
    int index = mUi->formLayout->count();
    QLabel *label = new QLabel(this);
    mUi->formLayout->setWidget(index, QFormLayout::LabelRole, label);
    label->setText(aName);

    QLineEdit* value = new QLineEdit(this);
    mUi->formLayout->setWidget(index, QFormLayout::FieldRole, value);
    Q_ASSERT(aValue.canConvert(QVariant::String));
    value->setText(aValue.toString());
    value->setObjectName(aName);
    QObject::connect(value, SIGNAL(textChanged(QString)), this, SLOT(notifySettingsChanged(QString)));
    }

void GuiElementList::notifySettingsChanged(const QString& aText)
    {
    emit paramChanged(sender()->objectName(), QVariant(aText));
    }

} // namespace media
