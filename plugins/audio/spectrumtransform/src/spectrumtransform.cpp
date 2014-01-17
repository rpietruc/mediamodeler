#include "spectrumtransform.h"
#include <QDebug>
#include <complex>
#include "filterframe.h"

namespace media {

SpectrumTransform::SpectrumTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    mSpectrumFrame.setResolution(512, M_PI);
    }

ElementBase::ParamList SpectrumTransform::getParams()
    {
    ParamList ret;
    ret["Resolution"] = mSpectrumFrame.getResolution();
    return ret;
    }

void SpectrumTransform::setParamValue(const QString& aName, const QVariant& aValue)
    {
    Q_UNUSED(aName);
    if (mSpectrumFrame.getResolution() != aValue.toInt())
        mSpectrumFrame.setResolution(aValue.toInt(), M_PI);
    }

void SpectrumTransform::process()
    {
    Q_ASSERT(mSpectrumFrame.getDimension(SpectrumFrame::Frequency).mResolution);
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == FilterFrame::Dimensions) &&
                (frame->getDimension(FilterFrame::Coefficients).mResolution > 2))
                {
                Q_ASSERT(frame->getDimension(FilterFrame::Time).mDelta);
                mSpectrumFrame.setFrameSamples(frame->getDimension(FilterFrame::Time).mResolution);
                mSpectrumFrame.setSampleRate(1.0/frame->getDimension(FilterFrame::Time).mDelta);
                mSpectrumFrame.setTimeStamp(frame->getDimension(FilterFrame::Time).mStartLocation);
                mSpectrumFrame.setSourceName(frame->getSourceName());

                double timeStamp = frame->getDimension(FilterFrame::Time).mStartLocation;

                int spcpoint[SpectrumFrame::Dimensions] = {0, 0};
                int filterpoint[FilterFrame::Dimensions] = {0, 0, 0};
                for (filterpoint[FilterFrame::Time] = spcpoint[SpectrumFrame::Time] = 0;
                     filterpoint[FilterFrame::Time] < frame->getDimension(FilterFrame::Time).mResolution;
                   ++filterpoint[FilterFrame::Time], ++spcpoint[SpectrumFrame::Time], timeStamp += frame->getDimension(FilterFrame::Time).mDelta)
                    {
                    qreal rad = mSpectrumFrame.getDimension(SpectrumFrame::Frequency).mStartLocation;
                    for (spcpoint[SpectrumFrame::Frequency] = 0;
                         spcpoint[SpectrumFrame::Frequency] < mSpectrumFrame.getDimension(SpectrumFrame::Frequency).mResolution;
                       ++spcpoint[SpectrumFrame::Frequency], rad += mSpectrumFrame.getDimension(SpectrumFrame::Frequency).mDelta)
                        {
                        std::complex<qreal> numerator;
                        std::complex<qreal> denominator;

                        // Z transform
                        for (filterpoint[FilterFrame::Coefficients] = 0;
                             filterpoint[FilterFrame::Coefficients] < frame->getDimension(FilterFrame::Coefficients).mResolution;
                           ++filterpoint[FilterFrame::Coefficients])
                            {
                            filterpoint[FilterFrame::Polynomial] = FilterFrame::Numerator;
                            numerator += std::complex<qreal>(frame->getSample(filterpoint)*cos(rad*filterpoint[FilterFrame::Coefficients]),
                                                            -frame->getSample(filterpoint)*sin(rad*filterpoint[FilterFrame::Coefficients]));
                            filterpoint[FilterFrame::Polynomial] = FilterFrame::Denominator;
                            denominator += std::complex<qreal>(frame->getSample(filterpoint)*cos(rad*filterpoint[FilterFrame::Coefficients]),
                                                              -frame->getSample(filterpoint)*sin(rad*filterpoint[FilterFrame::Coefficients]));
                            }
                        mSpectrumFrame.setSample(spcpoint, abs(numerator/denominator));
                        }
                    }
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
