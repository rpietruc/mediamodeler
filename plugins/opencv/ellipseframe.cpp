#include "ellipseframe.h"

namespace media {

EllipseFrame::EllipseFrame(const FrameBase& aFrame)
    {
    if ((aFrame.getMaxDimension() == Dimensions) &&
        (aFrame.getDimensionT(ParamNo).mResolution == VectorSize))
        {
        setMaxVectors(aFrame.getDimensionT(VectorNo).mResolution);
        setVectorSizeT(VectorSize);
        int point[Dimensions] = {0, 0};
        for (point[VectorNo] = 0; point[VectorNo] < aFrame.getDimensionT(VectorNo).mResolution; ++point[VectorNo])
            for (point[ParamNo] = 0; point[ParamNo] < aFrame.getDimensionT(ParamNo).mResolution; ++point[ParamNo])
                setSampleT(point, aFrame.getSampleT(point));
        }
    }

cv::RotatedRect EllipseFrame::getRect(int aIndex) const
    {
    cv::RotatedRect rect;

    int point[Dimensions] = {aIndex, CenterX};
    rect.center.x = getSampleT(point);
    point[ParamNo] = CenterY;
    rect.center.y = getSampleT(point);
    point[ParamNo] = SizeWidth;
    rect.size.width = getSampleT(point);
    point[ParamNo] = SizeHeight;
    rect.size.height = getSampleT(point);
    point[ParamNo] = AngleRotate;
    rect.angle = getSampleT(point);

    return rect;
    }

} // namespace media
