#ifndef GUIELEMENTBASE_H
#define GUIELEMENTBASE_H

#include <elementbase.h>
#include <QWidget>

namespace media {

class MEDIAMODELSHARED_EXPORT GuiElementBase : public QWidget
    {
    Q_OBJECT

public:
    GuiElementBase(QWidget *aParent) : QWidget(aParent) {}
    virtual void init(const ElementBase::ParamList& aParams)
        {
        Q_UNUSED(aParams);
        }

signals:
    void paramChanged(const QString& aName, const QVariant& aValue);
    };

} // namespace media

#endif // GUIELEMENTBASE_H
