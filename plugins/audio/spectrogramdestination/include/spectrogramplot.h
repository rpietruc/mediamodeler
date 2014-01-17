#ifndef SPECTROGRAMPLOT_H
#define SPECTROGRAMPLOT_H

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_spectrogram.h>

namespace Ui {
    class SpectrogramPlot;
}

class SpectrogramPlot : public QwtPlot
    {
    Q_OBJECT

public:
    explicit SpectrogramPlot(QwtRasterData *aData, QWidget *aParent = 0);

public Q_SLOTS:
    void showContour(bool on);
    void showSpectrogram(bool on);

#if (QT_VERSION >= 0x060000) && (! defined QT_NO_PRINTER)
    void printPlot();
#endif

private:
    Ui::SpectrogramPlot *ui;
    QwtPlotSpectrogram *mSpectrogram;
    };

#endif // SPECTROGRAMPLOT_H
