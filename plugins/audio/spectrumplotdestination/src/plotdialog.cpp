#include "plotdialog.h"
#include "ui_plotdialog.h"
#include <qcustomplot.h>

PlotDialog::PlotDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlotDialog),
    mLastKey(0)
    {
    ui->setupUi(this);

#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
    QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data plot needs functions that are available with Qt 4.7 to work properly");
#endif
    ui->customPlot->setBackground(QPixmap("../res/solarpanels.jpg"));

    // include this section to fully disable antialiasing for higher performance:
    ui->customPlot->setNotAntialiasedElements(QCP::aeAll);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    ui->customPlot->xAxis->setTickLabelFont(font);
    ui->customPlot->yAxis->setTickLabelFont(font);
    ui->customPlot->legend->setFont(font);

    ui->customPlot->xAxis->setAutoTickStep(false);
//    ui->customPlot->setupFullAxesBox();

    // make left and bottom axes transfer their ranges to right and top axes:
    QObject::connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    QObject::connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    ui->statusBar->clearMessage();
    ui->customPlot->replot();
    }

void PlotDialog::setTitle(const QString &aWindowTitle)
    {
    setWindowTitle(aWindowTitle);
    }

PlotDialog::~PlotDialog()
    {
    delete ui;
    }

void PlotDialog::setGraphNo(int aNo)
    {
    while (ui->customPlot->graphCount() < aNo)
        {
        QCPGraph *graph = ui->customPlot->addGraph();
        Q_ASSERT(graph);
        graph->setPen(QPen(getColor(ui->customPlot->graphCount() - 1)));
        }
    }

void PlotDialog::initTimeRange(double aKey, double aRange)
    {
    // remove data of lines that's outside visible range:
    for (int i = 0; i < ui->customPlot->graphCount(); ++i)
        ui->customPlot->graph(i)->removeDataBefore(aKey - aRange);
    // make aKey axis range scroll with the data at a constant range:
    ui->customPlot->xAxis->setRange(aKey, aRange, Qt::AlignRight);
    mLastKey = aKey;
    }

void PlotDialog::initKeyRange(double aMin, double aMax)
    {
    for (int i = 0; i < ui->customPlot->graphCount(); ++i)
        ui->customPlot->graph(i)->clearData();
    ui->customPlot->xAxis->setRange(aMin, aMax);
    mLastKey = aMax;
    }

void PlotDialog::appendSample(double aKey, double aValue, int aIndex)
    {
    Q_ASSERT(ui->customPlot->graphCount() > aIndex);
    ui->customPlot->graph(aIndex)->addData(aKey, aValue);
    if (mLastKey == aKey)
        {
        // rescale value (vertical) axis to fit the current data:
        for (int i = 0; i < ui->customPlot->graphCount(); ++i)
            ui->customPlot->graph(i)->rescaleValueAxis(i);
        ui->customPlot->replot();
        }
    }

QColor PlotDialog::getColor(int aIndex)
    {
    switch (aIndex)
        {
    case 0: return Qt::red;
    case 1: return Qt::green;
    case 2: return Qt::blue;
    case 3: return Qt::cyan;
    case 4: return Qt::magenta;
    case 5: return Qt::yellow;
    case 6: return Qt::darkRed;
    case 7: return Qt::darkGreen;
    case 8: return Qt::darkBlue;
    case 9: return Qt::darkCyan;
    case 10: return Qt::darkMagenta;
    case 11: return Qt::darkYellow;
    case 12: return Qt::darkGray;
    case 13: return Qt::gray;
    case 14: return Qt::lightGray;
    default: return Qt::black;
        }
    }
