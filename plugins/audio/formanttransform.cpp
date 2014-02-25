#include "formanttransform.h"
#include <QDebug>
#include "spectrumframe.h"

namespace media {

FormantTransform::FormantTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("maxFormants", 10);
    }

void FormantTransform::process()
    {
    mFormantFrame.setFormantsNo(property("maxFormants").toInt());
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == SpectrumFrame::Dimensions) &&
                (frame->getDimension(SpectrumFrame::Frequency).mResolution > 2))
                {
                mFormantFrame.setFrameSamples(frame->getDimension(SpectrumFrame::Time).mResolution);
                mFormantFrame.setSampleRate(1/frame->getDimension(SpectrumFrame::Time).mDelta);
                mFormantFrame.setTimeStamp(frame->getDimension(SpectrumFrame::Time).mStartLocation);
                mFormantFrame.setSourceName(frame->getSourceName());

                int spoint[SpectrumFrame::Dimensions] = {0, 0};
                int fpoint[FormantFrame::Dimensions] = {0, 0};
                for (spoint[SpectrumFrame::Time] = fpoint[SpectrumFrame::Time] = 0;
                     spoint[SpectrumFrame::Time] < frame->getDimension(SpectrumFrame::Time).mResolution;
                   ++spoint[SpectrumFrame::Time], ++fpoint[SpectrumFrame::Time])
                    {
                    QVector<qreal> spectrumFirstDerivative(frame->getDimension(SpectrumFrame::Frequency).mResolution - 1);
                    spoint[SpectrumFrame::Frequency] = 0;
                    for (int i = 0; i < spectrumFirstDerivative.size(); ++i)
                        {
                        qreal amplitude1 = frame->getSample(spoint);
                        ++spoint[SpectrumFrame::Frequency];
                        qreal amplitude2 = frame->getSample(spoint);
                        spectrumFirstDerivative[i] = amplitude2 - amplitude1;
                        }

                    QVector<qreal> spectrumSecondDerivative(spectrumFirstDerivative.size() - 1);
                    spectrumSecondDerivative[0] = spectrumFirstDerivative[1] - spectrumFirstDerivative[0];
                    bool lowering = false;
                    fpoint[FormantFrame::Formants] = 0;
                    for (int i = 1; i < spectrumSecondDerivative.size(); ++i)
                        {
                        spectrumSecondDerivative[i] = spectrumFirstDerivative[i + 1] - spectrumFirstDerivative[i];
                        if (spectrumSecondDerivative[i] < spectrumSecondDerivative[i - 1])
                            lowering = true;
                        else if (spectrumSecondDerivative[i] > spectrumSecondDerivative[i - 1])
                            {
                            if (lowering)
                                {
                                if (fpoint[FormantFrame::Formants] < mFormantFrame.getDimension(FormantFrame::Formants).mResolution)
                                    {
                                    qreal frequency = frame->getDimension(SpectrumFrame::Frequency).mStartLocation
                                                    + (i - 1)*frame->getDimension(SpectrumFrame::Frequency).mDelta;
                                    mFormantFrame.setSample(fpoint, frequency);
                                    ++fpoint[FormantFrame::Formants];
                                    }
                                else Q_ASSERT(0 && "not enough space for formants");
                                }
                            lowering = false;
                            }
                        }
                    }
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
