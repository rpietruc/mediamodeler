#include "spectrumtransform.h"
#include <QDebug>
#include <complex>
#include "filterframe.h"

namespace media {

SpectrumTransform::SpectrumTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("resolution", 512);
    }

void SpectrumTransform::process()
    {
    if (mSpectrumFrame.getResolution() != property("resolution").toInt())
        mSpectrumFrame.setResolution(property("resolution").toInt(), M_PI);

    Q_ASSERT(mSpectrumFrame.getDimensionT(SpectrumFrame::Frequency).mResolution);
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == FilterFrame::Dimensions) &&
                (frame->getDimensionT(FilterFrame::Coefficients).mResolution > 2))
                {
                Q_ASSERT(frame->getDimensionT(FilterFrame::Time).mDelta);
                mSpectrumFrame.setFrameSamples(frame->getDimensionT(FilterFrame::Time).mResolution);
                mSpectrumFrame.setSampleRate(1.0/frame->getDimensionT(FilterFrame::Time).mDelta);
                mSpectrumFrame.setTimeStamp(frame->getDimensionT(FilterFrame::Time).mStartLocation);
                mSpectrumFrame.setSourceName(frame->getSourceName());

                double timeStamp = frame->getDimensionT(FilterFrame::Time).mStartLocation;

                int spcpoint[SpectrumFrame::Dimensions] = {0, 0};
                int filterpoint[FilterFrame::Dimensions] = {0, 0, 0};
                for (filterpoint[FilterFrame::Time] = spcpoint[SpectrumFrame::Time] = 0;
                     filterpoint[FilterFrame::Time] < frame->getDimensionT(FilterFrame::Time).mResolution;
                   ++filterpoint[FilterFrame::Time], ++spcpoint[SpectrumFrame::Time], timeStamp += frame->getDimensionT(FilterFrame::Time).mDelta)
                    {
                    qreal rad = mSpectrumFrame.getDimensionT(SpectrumFrame::Frequency).mStartLocation;
                    for (spcpoint[SpectrumFrame::Frequency] = 0;
                         spcpoint[SpectrumFrame::Frequency] < mSpectrumFrame.getDimensionT(SpectrumFrame::Frequency).mResolution;
                       ++spcpoint[SpectrumFrame::Frequency], rad += mSpectrumFrame.getDimensionT(SpectrumFrame::Frequency).mDelta)
                        {
                        std::complex<qreal> numerator;
                        std::complex<qreal> denominator;

                        // Z transform
                        for (filterpoint[FilterFrame::Coefficients] = 0;
                             filterpoint[FilterFrame::Coefficients] < frame->getDimensionT(FilterFrame::Coefficients).mResolution;
                           ++filterpoint[FilterFrame::Coefficients])
                            {
                            filterpoint[FilterFrame::Polynomial] = FilterFrame::Numerator;
                            numerator += std::complex<qreal>(frame->getSampleT(filterpoint)*cos(rad*filterpoint[FilterFrame::Coefficients]),
                                                            -frame->getSampleT(filterpoint)*sin(rad*filterpoint[FilterFrame::Coefficients]));
                            filterpoint[FilterFrame::Polynomial] = FilterFrame::Denominator;
                            denominator += std::complex<qreal>(frame->getSampleT(filterpoint)*cos(rad*filterpoint[FilterFrame::Coefficients]),
                                                              -frame->getSampleT(filterpoint)*sin(rad*filterpoint[FilterFrame::Coefficients]));
                            }
                        mSpectrumFrame.setSampleT(spcpoint, abs(numerator/denominator));
                        }
                    }
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
