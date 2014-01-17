#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spectrumframe.h"

class SpectrogramData;
class SpectrogramPlot;

class SpectrogramWindow: public QMainWindow
    {
    Q_OBJECT

public:
    SpectrogramWindow(const media::SpectrumFrame& aFrame, QWidget* aParent = NULL);

public slots:
    void replot();

private:
    SpectrogramData *mData;
    SpectrogramPlot *mPlot;
    };


#endif // MAINWINDOW_H
