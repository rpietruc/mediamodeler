#include "spectrogramdestination.h"
#include <QDebug>
#include "spectrumframe.h"
#include "spectrogramwindow.h"

namespace media {

SpectrogramDestination::SpectrogramDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mSpectrogram(NULL)
    {
    }

SpectrogramDestination::~SpectrogramDestination()
    {
    delete mSpectrogram;
    }

void SpectrogramDestination::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == SpectrumFrame::Dimensions) &&
                (frame->getDimensionT(SpectrumFrame::Time).mResolution == 1) &&
                (frame->getDimensionT(SpectrumFrame::Frequency).mResolution > 2))
                {
                mSpectrumFrame.setSourceName(frame->getSourceName());
                if (!mSpectrogram)
                    {
                    Q_ASSERT(frame->getDimensionT(SpectrumFrame::Time).mDelta);
                    mSpectrumFrame.setSampleRate(1.0/frame->getDimensionT(SpectrumFrame::Time).mDelta);
                    mSpectrumFrame.setTimeStamp(frame->getDimensionT(SpectrumFrame::Time).mStartLocation);
                    mSpectrumFrame.setFrequencyResolution(frame->getDimensionT(SpectrumFrame::Frequency).mResolution, frame->getDimensionT(SpectrumFrame::Frequency).mResolution*frame->getDimensionT(SpectrumFrame::Frequency).mDelta);
                    mSpectrumFrame.setFrameSamples(512);
                    mSpectrogram = new SpectrogramWindow(mSpectrumFrame);
                    mSpectrogram->resize(600, 400);
                    mSpectrogram->show();
                    QObject::connect(this, SIGNAL(dataUpdated()), mSpectrogram, SLOT(replot()));
                    }
                mSpectrumFrame += *frame;
                if (mSpectrumFrame.getDimensionT(SpectrumFrame::Time).mResolution > 1)
                    emit dataUpdated();
                emit framesProcessed();
                }
            }
    }

} // namespace media
