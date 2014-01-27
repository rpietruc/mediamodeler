#ifndef BOXPLOT_H
#define BOXPLOT_H

#include <QWidget>
#include <QList>

class QHBoxLayout;
class QProgressBar;

class BoxPlot : public QWidget
    {
    Q_OBJECT

public:
    explicit BoxPlot(QWidget *parent = 0);
    ~BoxPlot();

    void setBoxNo(int aBoxNo);
    void setValue(int aIndex, double aValue);

    void setMaxValue(double aMaxValue) { mMaxValue = aMaxValue; }

    double maxValue() const { return mMaxValue; }
    int boxNo() const { return mBars.size(); }

private:
    double mMaxValue;
    QHBoxLayout *mLayout;
    QList<QProgressBar*> mBars;
    };

#endif // BOXPLOT_H
