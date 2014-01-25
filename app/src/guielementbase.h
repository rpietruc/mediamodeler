#ifndef GUIELEMENTBASE_H
#define GUIELEMENTBASE_H

#include <QWidget>

namespace media {

class GuiElementBase : public QWidget
    {
    Q_OBJECT

public:
    GuiElementBase(QWidget *aParent) : QWidget(aParent) {}
    virtual void addProperty(const QString &aName, const QVariant &aValue)
        {
        Q_UNUSED(aName);
        Q_UNUSED(aValue);
        }

signals:
    void paramChanged(const QString& aName, const QVariant& aValue);
    };

} // namespace media

#endif // GUIELEMENTBASE_H
