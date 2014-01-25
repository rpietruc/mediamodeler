#include "picturecontourtransform.h"

using namespace cv;
using namespace std;

namespace media {

PictureContourTransform::PictureContourTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mGrayFrame(1),
    mMethod(CV_CHAIN_APPROX_SIMPLE)
    {
    }

//ElementBase::ParamList PictureContourTransform::getParams() const
//    {
//    ParamList ret;
//    ret["Method"] =  mMethod;
//    return ret;
//    }

//void PictureContourTransform::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    Q_UNUSED(aName);
//    mMethod = aValue.toInt();
//    }

void PictureContourTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if ((frame->getMaxDimension() == IplImageFrame::Dimensions) || (frame->getMaxDimension() == (IplImageFrame::Dimensions - 1)))
                {
                mSrcFrame.resizeAndCopyFrame(*frame);
                IplImage* srcImg = mSrcFrame;
                mGrayFrame.resizeAndCopyImage(*srcImg);

                vector<vector<Point> > contours;
                findContours(Mat(mGrayFrame), contours, CV_RETR_LIST, mMethod);

                mPointsFrameSet.clear();
                foreach (vector<Point> contour, contours)
                    {
//                    if (contour.size() > 30)
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

