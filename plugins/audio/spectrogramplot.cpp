#include "spectrogramplot.h"
#include <qprinter.h>
#include <qprintdialog.h>
#include <qwt/qwt_color_map.h>
#include <qwt/qwt_plot_spectrogram.h>
#include <qwt/qwt_scale_widget.h>
#include <qwt/qwt_scale_draw.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_plot_panner.h>
#include <qwt/qwt_plot_layout.h>
#if (QT_VERSION >= 0x060000)
#   include <qwt/qwt_plot_renderer.h>
#endif
#include <qwt/qwt_plot_canvas.h>

class MyZoomer: public QwtPlotZoomer
    {
public:
    MyZoomer(QWidget *canvas):
        QwtPlotZoomer(canvas)
        {
        setTrackerMode(AlwaysOn);
        }

    virtual QwtText trackerTextF(const QPointF &pos) const
        {
        QColor bg(Qt::white);
        bg.setAlpha(200);

        QwtText text = QwtPlotZoomer::trackerTextF(pos);
        text.setBackgroundBrush(QBrush(bg));
        return text;
        }
    };

class ColorMap: public QwtLinearColorMap
    {
public:
    ColorMap() :
        QwtLinearColorMap(Qt::darkCyan, Qt::red)
        {
        addColorStop(0.1, Qt::cyan);
        addColorStop(0.6, Qt::green);
        addColorStop(0.95, Qt::yellow);
        }
    };

SpectrogramPlot::SpectrogramPlot(QwtRasterData *aData, QWidget *aParent):
    QwtPlot(aParent)
    {
    mSpectrogram = new QwtPlotSpectrogram;
    mSpectrogram->setRenderThreadCount(0); // use system specific thread count

    mSpectrogram->setColorMap(new ColorMap());

    mSpectrogram->setData(aData);
    mSpectrogram->attach(this);

    QList<double> contourLevels;
    for (double level = 0.5; level < 10.0; level += 1.0)
        contourLevels += level;
    mSpectrogram->setContourLevels(contourLevels);

    const QwtInterval zInterval = mSpectrogram->data()->interval(Qt::ZAxis);
    // A color bar on the right axis
    QwtScaleWidget *rightAxis = axisWidget(QwtPlot::yRight);
    rightAxis->setTitle("Intensity");
    rightAxis->setColorBarEnabled(true);
    rightAxis->setColorMap( zInterval, new ColorMap());

    setAxisScale(QwtPlot::yRight, zInterval.minValue(), zInterval.maxValue() );
    enableAxis(QwtPlot::yRight);

    plotLayout()->setAlignCanvasToScales(true);
    replot();

    // LeftButton for the zooming
    // MidButton for the panning
    // RightButton: zoom out by 1
    // Ctrl+RighButton: zoom out to full size

    QwtPlotZoomer* zoomer = new MyZoomer(canvas());
    zoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
    zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);

    QwtPlotPanner *panner = new QwtPlotPanner(canvas());
    panner->setAxisEnabled(QwtPlot::yRight, false);
    panner->setMouseButton(Qt::MidButton);

    // Avoid jumping when labels with more/less digits
    // appear/disappear when scrolling vertically

    const QFontMetrics fm(axisWidget(QwtPlot::yLeft)->font());
    QwtScaleDraw *sd = axisScaleDraw(QwtPlot::yLeft);
    sd->setMinimumExtent(fm.width("100.00"));

    const QColor c(Qt::darkBlue);
    zoomer->setRubberBandPen(c);
    zoomer->setTrackerPen(c);
    }

void SpectrogramPlot::showContour(bool on)
    {
    mSpectrogram->setDisplayMode(QwtPlotSpectrogram::ContourMode, on);
    replot();
    }

void SpectrogramPlot::showSpectrogram(bool on)
    {
    mSpectrogram->setDisplayMode(QwtPlotSpectrogram::ImageMode, on);
    mSpectrogram->setDefaultContourPen(on ? QPen() : QPen(Qt::NoPen));
    replot();
    }

#if (QT_VERSION >= 0x060000) && (! defined QT_NO_PRINTER)

void SpectrogramPlot::printPlot()
    {
#if 1
    QPrinter printer;
#else
    QPrinter printer(QPrinter::HighResolution);
#endif
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFileName("spectrogram.pdf");
    QPrintDialog dialog(&printer);
    if (dialog.exec())
        {
        QwtPlotRenderer renderer;

        renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, false);
        renderer.setLayoutFlag(QwtPlotRenderer::KeepFrames, true);

        renderer.renderTo(this, printer);
        }
    }

#endif
