#ifndef ELEMENTBOX_H
#define ELEMENTBOX_H

#include <QGroupBox>
#include "elementbase.h"

namespace media {

class ElementBox : public QGroupBox
    {
    Q_OBJECT

public:
    explicit ElementBox(const ElementBase* aElement, QWidget *aParent = 0);

signals:
    void settingChanged(const QString &aSection, const QString &aName, const QVariant &aValue);

private slots:
    void paramChanged(const QString &aName, const QVariant &aValue) { emit settingChanged(title(), aName, aValue); }
    };

} // namespace media

#endif // ELEMENTBOX_H
