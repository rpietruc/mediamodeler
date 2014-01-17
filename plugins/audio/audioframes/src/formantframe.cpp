#include "formantframe.h"

namespace media {

FormantFrame::FormantFrame()
    {
    mDimensions[Formants].mStartLocation = 0;
    mDimensions[Formants].mDelta = 1;
    }

void FormantFrame::setSampleRate(int aSampleRate)
    {
    Q_ASSERT(aSampleRate);
    mDimensions[Time].mDelta = 1.0/aSampleRate;
    }

} // namespace media
