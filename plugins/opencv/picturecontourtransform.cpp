#include "picturecontourtransform.h"

using namespace cv;
using namespace std;

namespace media {

PictureContourTransform::PictureContourTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("method", CV_CHAIN_APPROX_SIMPLE);
    setProperty("minlen", 30);
    }

void PictureContourTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            PictureGrayFrame grayImg;
            if (grayImg.isCopyable(*frame))
                {
                grayImg.resizeAndCopyFrame(*frame);
                vector<vector<Point> > contours;
                findContours(Mat(grayImg), contours, CV_RETR_LIST, qBound((int)CV_CHAIN_APPROX_NONE, property("method").toInt(), (int)CV_LINK_RUNS));

                mPointsFrameSet.clear();
                foreach (vector<Point> contour, contours)
                    {
                    if ((int)contour.size() > property("minlen").toInt())
                        {
                        PointsFrame pointsFrame;
                        pointsFrame = contour;
                        pointsFrame.setSourceName(frame->getSourceName());
                        mPointsFrameSet.push_back(pointsFrame);
                        }
                    }
                emit framesReady();
                break;
                }
            }
    }

} // namespace media

