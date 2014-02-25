#include "picturedrawtransform.h"

using namespace cv;
using namespace std;

namespace media {

PictureDrawTransform::PictureDrawTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    }

void PictureDrawTransform::process()
    {
    vector<vector<Point> > contours;
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (frame->getMaxDimension() == IplImageFrame::Dimensions)
                {
                mPictureFrame.resizeAndCopyFrame(*frame);
                mPictureFrame.setSourceName(frame->getSourceName());
                }
            else if ((frame->getMaxDimension() == PointsFrame::Dimensions) &&
                (frame->getDimension(PointsFrame::Axis).mResolution == PointsFrame::MaxAxis))
                {
                vector<Point> contour;
                contour.resize(frame->getDimension(PointsFrame::Index).mResolution);
                int point[PointsFrame::Dimensions] = {0, 0};
                for (point[PointsFrame::Index] = 0; point[PointsFrame::Index] < frame->getDimension(PointsFrame::Index).mResolution; ++point[PointsFrame::Index])
                    {
                    point[PointsFrame::Axis] = PointsFrame::XAxis;
                    contour[point[PointsFrame::Index]].x = frame->getSample(point);
                    point[PointsFrame::Axis] = PointsFrame::YAxis;
                    contour[point[PointsFrame::Index]].y = frame->getSample(point);
                    }
                contours.push_back(contour);
                }
            }
    drawContours(Mat(mPictureFrame), contours, -1, Scalar(0, 0, 0), -1, 8, noArray());
    emit framesReady();
    }

} // namespace media
