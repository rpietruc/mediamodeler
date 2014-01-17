#include "acoustictubetransform.h"
#include "filterframe.h"
#include <vector>

using namespace acoustics;
using namespace std;

namespace media {

AcousticTubeTransform::AcousticTubeTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mFilterConverter(NULL)
    {
    }

void AcousticTubeTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == FilterFrame::Dimensions) &&
                (frame->getDimension(FilterFrame::Coefficients).mResolution > 1))
                {
                if (!mFilterConverter)
                    mFilterConverter = new CFilterConverter<float>(frame->getDimension(FilterFrame::Coefficients).mResolution - 1);

                mAreasFrame.setMaxVectors(frame->getDimension(FilterFrame::Coefficients).mResolution - 1);
                mAreasFrame.setMaxSamples(frame->getDimension(FilterFrame::Time).mResolution);

                int point[FilterFrame::Dimensions] = {0, 0, 0};
                point[FilterFrame::Polynomial] = FilterFrame::Denominator;
                for (; point[FilterFrame::Time] < frame->getDimension(FilterFrame::Time).mResolution; ++point[FilterFrame::Time])
                    {
                    vector<float> lpc(frame->getDimension(FilterFrame::Coefficients).mResolution - 1);
                    vector<float> areas(frame->getDimension(FilterFrame::Coefficients).mResolution - 1);
                    for (point[FilterFrame::Coefficients] = 1; point[FilterFrame::Coefficients] < frame->getDimension(FilterFrame::Coefficients).mResolution; ++point[FilterFrame::Coefficients])
                        lpc[point[FilterFrame::Coefficients] - 1] = -frame->getSample(point);
                    if ((mFilterConverter->SetLPC(lpc)) &&
                        (mFilterConverter->GetAreas(areas.data(), areas.size()) == areas.size()))
                            {
                            for (point[FilterFrame::Coefficients] = 0; point[FilterFrame::Coefficients] < mAreasFrame.getDimension(FilterFrame::Coefficients).mResolution; ++point[FilterFrame::Coefficients])
                                mAreasFrame.setSample(point, areas[point[FilterFrame::Coefficients]]);
                            mAreasFrame.setSourceName(frame->getSourceName());
                            }
                    }
                emit framesReady();
                break;
                }
            }
    }

} // namespace media
