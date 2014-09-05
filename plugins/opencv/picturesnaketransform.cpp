#include "picturesnaketransform.h"
#include <opencv2/legacy/legacy.hpp>
#include <vector>

#define pi 3.14159265358979

using namespace std;

namespace media {

PictureSnakeTransform::PictureSnakeTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("neighborhoodSize", 11); // neighborhood size, must be odd

    // weight of continuity energy
    // continuity energy refers to the distance between a pixel
    // and its neighboring snake point with respect to all the snake points;
    // it is calculated by subtracting the average distance
    // (between all snake points)
    // from the distance between the pixel and the previous snake point
    setProperty("continuityEnergyWeight", 0.35);

    // weight of curvature energy
    // curvature energy refers to the amount curvature between a pixel
    // and the neighboring snake points
    // this can be calculated using the second derivative, which corresponds to:
    // (next snake point - pixel) - (pixel - previous snake point)
    setProperty("curvatureEnergyWeight", 0.21);

    // weight of image energy
    // gradient energy merely refers to negative
    // the size of the gradient of the pixel
    setProperty("imageEnergyWeight", 0.62);
    }

void PictureSnakeTransform::process()
    {
    PictureRGBFrame srcFrame;

    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (srcFrame.isCopyable(*frame))
                srcFrame.resizeAndCopyFrame(*frame);
            else if (mPointsFrame.isCopyable(*frame))
                mPointsFrame.resizeAndCopyFrame(*frame);
            }

    if (!srcFrame.isEmpty())
        {
        CvSize searchWindow = { property("neighborhoodSize").toInt(), property("neighborhoodSize").toInt() };
        CvTermCriteria termCriteria = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1.e-15f);
        float continuityEnergyWeight = property("continuityEnergyWeight").toFloat();
        float curvatureEnergyWeight = property("curvatureEnergyWeight").toFloat();
        float imageEnergyWeight = property("imageEnergyWeight").toFloat();

        vector<CvPoint> points;
        if (mPointsFrame.isEmpty())
            {
            cv::Point center(srcFrame.getDimensionT(PictureRGBFrame::Width).mResolution/2, srcFrame.getDimensionT(PictureRGBFrame::Height).mResolution/2);
            cv::Size radius(srcFrame.getDimensionT(PictureRGBFrame::Width).mResolution/4, srcFrame.getDimensionT(PictureRGBFrame::Height).mResolution/4);
            points.resize(sqrt(srcFrame.getDimensionT(PictureRGBFrame::Width).mResolution) + sqrt(srcFrame.getDimensionT(PictureRGBFrame::Height).mResolution));
            for (int i = 0; i < (int)points.size(); ++i)
                {
                points[i].x = center.x + static_cast<int>(round(radius.width*cos(i*((2*pi)/points.size()))));
                points[i].y = center.y - radius.height + static_cast<int>(round(radius.height*sin(i*((2*pi)/points.size()))));
                }
            }
        else
            mPointsFrame.copyTo(points);
        cvSnakeImage((IplImage*)srcFrame, points.data(), (int)points.size(), &continuityEnergyWeight, &curvatureEnergyWeight, &imageEnergyWeight, CV_VALUE, searchWindow, termCriteria, 0);
        mPointsFrame = points;
        }
    emit framesReady();
    }

} // namespace media
