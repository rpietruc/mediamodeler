#ifndef ELEMENTBOX_H
#define ELEMENTBOX_H

#include <QGroupBox>

namespace media {

class PropertiesBox : public QGroupBox
    {
    Q_OBJECT

public:
    explicit PropertiesBox(QObject *aPropertiesObject, QWidget *aParent = 0);

signals:
    void settingChanged(const QString &aSection, const QString &aName, const QVariant &aValue);

private slots:
    void paramChanged(const QString &aName, const QVariant &aValue);

private:
    QObject *mPropertiesObject;
    };

} // namespace media

#endif // ELEMENTBOX_H
