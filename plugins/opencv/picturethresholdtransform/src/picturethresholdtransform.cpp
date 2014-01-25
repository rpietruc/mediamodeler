#include "picturethresholdtransform.h"

namespace media {

PictureThresholdTransform::PictureThresholdTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mSrcFrame(1),
    mPictureFrame(1),
    mThreshold(1),
    mMaxValue(100),
    mType(CV_THRESH_BINARY)
    {
    }

//ElementBase::ParamList PictureThresholdTransform::getParams() const
//    {
//    ParamList ret;
//    ret["Threshold"] =  mThreshold;
//    ret["Max Value"] = mMaxValue;
//    ret["Type"] = mType;
//    return ret;
//    }

//void PictureThresholdTransform::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    if (aName == "Threshold")
//        mThreshold = aValue.toDouble();
//    else if (aName == "Max Value")
//        mMaxValue = aValue.toDouble();
//    else if (aName == "Type")
//        mType = aValue.toInt();
//    }

void PictureThresholdTransform::process()
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
                mPictureFrame.resize(srcImg->width, srcImg->height);

                cvThreshold(srcImg, mPictureFrame, mThreshold, mMaxValue, mType);

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
