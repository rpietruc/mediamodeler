#ifndef GUIELEMENTLIST_H
#define GUIELEMENTLIST_H

#include <QWidget>
#include "guielementbase.h"

namespace Ui {
    class GuiElementList;
}

namespace media {

class GuiElementList : public GuiElementBase
    {
    Q_OBJECT

public:
    explicit GuiElementList(QWidget *aParent = 0);
    ~GuiElementList();

    void addProperty(const QString &aName, const QVariant &aValue);

private slots:
    void notifySettingsChanged(const QString& aText);

private:
    Ui::GuiElementList *mUi;
    };

} // namespace media

#endif // GUIELEMENTLIST_H
