#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include <QTimer>

namespace Ui {
    class PlotDialog;
}

class PlotDialog : public QDialog
    {
    Q_OBJECT
    
public:
    explicit PlotDialog(QWidget *parent = 0);
    ~PlotDialog();

    void setTitle(const QString &aWindowTitle);
    void setGraphNo(int aNo);
    void initTimeRange(double aKey, double aRange);
    void initKeyRange(double aMin, double aMax);

public slots:
    void appendSample(double aKey, double aValue, int aIndex);

private:
    QColor getColor(int aIndex);

private:
    Ui::PlotDialog *ui;
    double mLastKey;
    };

#endif // PLOTDIALOG_H
