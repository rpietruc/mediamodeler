#include "pictureedgedetectiontransform.h"

namespace media {

PictureEdgeDetectionTransform::PictureEdgeDetectionTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mGrayFrame(1),
    mPictureFrame(1),
    mThreshold1(50),
    mThreshold2(100),
    mApertureSize(5)
    {
    }

//ElementBase::ParamList PictureEdgeDetectionTransform::getParams() const
//    {
//    ParamList ret;
//    ret["Threshold 1"] =  mThreshold1;
//    ret["Threshold 2"] =  mThreshold2;
//    ret["Aperture Size"] = mApertureSize;
//    return ret;
//    }

//void PictureEdgeDetectionTransform::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    if (aName == "Threshold 1")
//        mThreshold1 = aValue.toDouble();
//    else if (aName == "Threshold 2")
//        mThreshold2 = aValue.toDouble();
//    else if (aName == "Aperture Size")
//        mApertureSize = aValue.toInt();
//    }

void PictureEdgeDetectionTransform::process()
    {
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (frame->getMaxDimension() == IplImageFrame::Dimensions)
                {
                mPictureFrame.setSourceName(frame->getSourceName());
                mSrcFrame.resizeAndCopyFrame(*frame);
                IplImage* srcImg = mSrcFrame;
                mGrayFrame.resizeAndCopyImage(*srcImg);
                mPictureFrame.resize(srcImg->width, srcImg->height);

                cvCanny(srcImg, mPictureFrame, mThreshold1, mThreshold2, mApertureSize);

                emit framesReady();
                break;
                }
            }
    }

} // namespace media

