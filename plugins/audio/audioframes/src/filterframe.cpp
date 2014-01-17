#include "filterframe.h"

namespace media {

FilterFrame::FilterFrame()
    {
    mDimensions[Coefficients].mDelta = 1.0;
    setMaxPlanes(2); // Numerator + Denominator
    }

void FilterFrame::setSampleRate(int aSampleRate)
    {
    Q_ASSERT(aSampleRate);
    mDimensions[Time].mDelta = 1.0/aSampleRate;
    }

} // namespace media
