#include "picturedrawtransform.h"

using namespace cv;
using namespace std;

namespace media {

PictureDrawTransform::PictureDrawTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("intensity", 0);
    setProperty("thickness", -1);
    }

void PictureDrawTransform::process()
    {
    mPictureFrame.clear();
    vector<vector<Point> > contours;
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (PointsFrame().isCopyable(*frame))
                {
                vector<Point> contour;
                contour.resize(frame->getDimensionT(PointsFrame::Index).mResolution);
                int point[PointsFrame::Dimensions] = {0, 0};
                for (point[PointsFrame::Index] = 0; point[PointsFrame::Index] < frame->getDimensionT(PointsFrame::Index).mResolution; ++point[PointsFrame::Index])
                    {
                    point[PointsFrame::Axis] = PointsFrame::XAxis;
                    contour[point[PointsFrame::Index]].x = frame->getSampleT(point);
                    point[PointsFrame::Axis] = PointsFrame::YAxis;
                    contour[point[PointsFrame::Index]].y = frame->getSampleT(point);
                    }
                contours.push_back(contour);
                }
            else if (mPictureFrame.isCopyable(*frame))
                {
                mPictureFrame.resizeAndCopyFrame(*frame);
                mPictureFrame.setSourceName(frame->getSourceName());
                }
            }
    if (!mPictureFrame.isEmpty())
        drawContours(Mat(mPictureFrame), contours, -1, Scalar(property("intensity").toDouble(), property("intensity").toDouble(), property("intensity").toDouble()), property("thickness").toInt(), 8, noArray());
    emit framesReady();
    }

} // namespace media
