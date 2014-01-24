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

    void init(const ElementBase::ParamList& aParams);

private slots:
    void notifySettingsChanged(const QString& aText);

private:
    Ui::GuiElementList *mUi;
    };

} // namespace media

#endif // GUIELEMENTLIST_H
