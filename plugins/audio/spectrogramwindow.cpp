#include "spectrogramwindow.h"
#include "spectrogramplot.h"
#include <QToolBar>
#include <QToolButton>
#include <qwt/qwt_raster_data.h>

using namespace media;

class SpectrogramData: public QwtRasterData
    {
public:
    SpectrogramData(const SpectrumFrame& aFrame) :
        mFrame(aFrame)
        {
        updateTimeInterval();
        updateFreqInterval();
        updateDataInterval();
        }

    void updateTimeInterval()
        {
        setInterval(Qt::XAxis, QwtInterval(mFrame.getDimensionT(SpectrumFrame::Time).mStartLocation,
                                           mFrame.getDimensionT(SpectrumFrame::Time).mStartLocation + mFrame.getDimensionT(SpectrumFrame::Time).mDelta*(mFrame.getDimensionT(SpectrumFrame::Time).mResolution - 1)));
        qDebug() << "time interval updated: x=(" << interval(Qt::XAxis).minValue() << ", " << interval(Qt::XAxis).maxValue()
                                      << ") y=(" << interval(Qt::YAxis).minValue() << ", " << interval(Qt::YAxis).maxValue() << ")";
        }

    void updateFreqInterval()
        {
        setInterval(Qt::YAxis, QwtInterval(mFrame.getDimensionT(SpectrumFrame::Frequency).mStartLocation,
                                           mFrame.getDimensionT(SpectrumFrame::Frequency).mStartLocation + mFrame.getDimensionT(SpectrumFrame::Frequency).mDelta*(mFrame.getDimensionT(SpectrumFrame::Frequency).mResolution - 1)));
        }

    void updateDataInterval()
        {
        setInterval(Qt::ZAxis, QwtInterval(0.0, 10.0));
        }

    virtual double value(double x, double y) const
        {
        int point[SpectrumFrame::Dimensions];
        point[SpectrumFrame::Time] = (x - interval(Qt::XAxis).minValue())/mFrame.getDimensionT(SpectrumFrame::Time).mDelta;
        point[SpectrumFrame::Frequency] = (y - interval(Qt::YAxis).minValue())/mFrame.getDimensionT(SpectrumFrame::Frequency).mDelta;
        return mFrame.getSampleT(point);
        }

private:
    const SpectrumFrame& mFrame;
    };

void SpectrogramWindow::replot()
    {
    mData->updateTimeInterval();
    mPlot->replot();
    }

SpectrogramWindow::SpectrogramWindow(const SpectrumFrame& aFrame, QWidget *parent):
    QMainWindow(parent)
    {
    mData = new SpectrogramData(aFrame);
    mPlot = new SpectrogramPlot(mData, this);

    setCentralWidget(mPlot);

    QToolBar *toolBar = new QToolBar(this);

    QToolButton *btnSpectrogram = new QToolButton(toolBar);
    btnSpectrogram->setText("Spectrogram");
    btnSpectrogram->setCheckable(true);
    btnSpectrogram->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(btnSpectrogram);
    QObject::connect(btnSpectrogram, SIGNAL(toggled(bool)), mPlot, SLOT(showSpectrogram(bool)));

    QToolButton *btnContour = new QToolButton(toolBar);
    btnContour->setText("Contour");
    btnContour->setCheckable(true);
    btnContour->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(btnContour);
    QObject::connect(btnContour, SIGNAL(toggled(bool)), mPlot, SLOT(showContour(bool)));

#if (QT_VERSION >= 0x060000) && (! defined QT_NO_PRINTER)
    QToolButton *btnPrint = new QToolButton(toolBar);
    btnPrint->setText("Print");
    btnPrint->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    toolBar->addWidget(btnPrint);
    QObject::connect(btnPrint, SIGNAL(clicked()), mPlot, SLOT(printPlot()) );
#endif

    addToolBar(toolBar);

    btnSpectrogram->setChecked(true);
    btnContour->setChecked(false);
    }
