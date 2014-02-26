#ifndef ELEMENTBOX_H
#define ELEMENTBOX_H

#include <QGroupBox>

class QTextBrowser;
class QTimer;

namespace media {

class PropertiesBox : public QGroupBox
    {
    Q_OBJECT

public:
    explicit PropertiesBox(QObject *aPropertiesObject, QWidget *aParent = 0);

signals:
    void settingChanged(const QString &aSection, const QString &aName, const QVariant &aValue);

public slots:
    void logMessage(int aPriority, const QString &aInfo);

private slots:
    void paramChanged(const QString &aName, const QVariant &aValue);
    void logTimestamp();

private:
    void log(const QColor &aColor, const QString &aInfo);

private:
    QObject *mPropertiesObject;
    QTextBrowser *mTextBrowser;
    QTimer *mTimeStamper;
    int mCountDown;
    };

} // namespace media

#endif // ELEMENTBOX_H
