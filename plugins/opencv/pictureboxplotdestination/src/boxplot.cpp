#include "src/boxplot.h"
#include <QProgressBar>
#include <QHBoxLayout>

BoxPlot::BoxPlot(QWidget *parent) :
    QWidget(parent),
    mLayout(new QHBoxLayout(this))
    {
//    setBoxNo(32);
    }

BoxPlot::~BoxPlot()
    {
    }

void BoxPlot::setBoxNo(int aBoxNo)
    {
    for (int i = 0; i < aBoxNo; ++i)
        {
        if (mBars.count() <= i)
            {
            mBars.push_back(new QProgressBar(this));
            mBars[i]->setOrientation(Qt::Vertical);
            mBars[i]->setValue(0);
            mBars[i]->setRange(0, 100);
            mLayout->addWidget(mBars[i]);
            }
        mBars[i]->show();
        }

    for (int i = aBoxNo; i < mBars.size(); ++i)
        mBars[i]->hide();
    }

void BoxPlot::setValue(int aIndex, double aValue)
    {
    if (aIndex < mBars.count())
        mBars[aIndex]->setValue(aValue*100.0/mMaxValue);
    }
