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
            PictureGrayFrame srcGrayFrame;
            if (srcGrayFrame.isCopyable(*frame))
                {
                srcGrayFrame.setSourceName(frame->getSourceName());
                srcGrayFrame.resizeAndCopyFrame(*frame);

                QSet<int> regions;
                int point[PictureGrayFrame::Dimensions] = {0, 0, 0};
                for (point[PictureGrayFrame::Height] = 0; point[PictureGrayFrame::Height] < srcGrayFrame.getDimensionT(PictureGrayFrame::Height).mResolution; ++point[PictureGrayFrame::Height])
                    for (point[PictureGrayFrame::Width] = 0; point[PictureGrayFrame::Width] < srcGrayFrame.getDimensionT(PictureGrayFrame::Width).mResolution; ++point[PictureGrayFrame::Width])
                        regions.insert(srcGrayFrame.getSampleT(point));

                mPointsFrameSet.clear();
//                int i = *(regions.begin() + qrand()%regions.size());
                foreach (int i, regions)
                    {
                    Mat region;
                    inRange(Mat(srcGrayFrame), i, i, region);

                    vector<vector<Point> > contours;
                    findContours(region, contours, CV_RETR_LIST, qBound((int)CV_CHAIN_APPROX_NONE, property("method").toInt(), (int)CV_LINK_RUNS));
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
                    }
                emit framesReady();
                break;
                }
            }
    }

} // namespace media

