#include "linesegmentframe.h"

namespace media {

LineSegmentFrame::LineSegmentFrame(const FrameBase& aFrame)
    {
    Q_ASSERT(isCopyable(aFrame));
    setMaxVectors(aFrame.getDimensionT(VectorNo).mResolution);
    setVectorSizeT(VectorSize);
    int point[Dimensions] = {0, 0};
    for (point[VectorNo] = 0; point[VectorNo] < aFrame.getDimensionT(VectorNo).mResolution; ++point[VectorNo])
        for (point[ParamNo] = 0; point[ParamNo] < aFrame.getDimensionT(ParamNo).mResolution; ++point[ParamNo])
            setSampleT(point, aFrame.getSampleT(point));
    }

} // namespace media
