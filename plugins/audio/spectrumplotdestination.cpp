#include "spectrumplotdestination.h"
#include <QDebug>
#include "spectrumframe.h"
#include <cmath>
#include "plotdialog.h"

namespace media {

SpectrumPlotDestination::SpectrumPlotDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mPlot(new PlotDialog)
    {
    mPlot->show();
    QObject::connect(this, SIGNAL(sampleReceived(double, double, int)), mPlot, SLOT(appendSample(double, double, int)), Qt::QueuedConnection);
    }

SpectrumPlotDestination::~SpectrumPlotDestination()
    {
    delete mPlot;
    }

void SpectrumPlotDestination::process()
    {
    Q_ASSERT(mPlot);
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == SpectrumFrame::Dimensions) &&
                (frame->getDimensionT(SpectrumFrame::Time).mResolution == 1) &&
                (frame->getDimensionT(SpectrumFrame::Frequency).mResolution > 2))
                {
                int point[SpectrumFrame::Dimensions] = {0, 0};
                mPlot->setGraphNo(1);
                double key = frame->getDimensionT(SpectrumFrame::Frequency).mStartLocation;
                for (point[SpectrumFrame::Frequency] = 0;
                     point[SpectrumFrame::Frequency] < frame->getDimensionT(SpectrumFrame::Frequency).mResolution;
                   ++point[SpectrumFrame::Frequency])
                    {
                    emit sampleReceived(key, frame->getSampleT(point), 0);
                    key += frame->getDimensionT(SpectrumFrame::Frequency).mDelta;
                    }
                mPlot->initKeyRange(frame->getDimensionT(SpectrumFrame::Frequency).mStartLocation, key - frame->getDimensionT(SpectrumFrame::Frequency).mDelta);
                mPlot->setTitle(frame->getSourceName());
                emit framesProcessed();
                }
            }
    }

} // namespace media
