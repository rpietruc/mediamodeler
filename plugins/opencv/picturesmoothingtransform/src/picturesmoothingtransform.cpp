#include "picturesmoothingtransform.h"

namespace media {

PictureSmoothingTransform::PictureSmoothingTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mSpatialRadius(2),
    mColorRadius(40)
    {
    }

//ElementBase::ParamList PictureSmoothingTransform::getParams() const
//    {
//    ParamList ret;
//    ret["Spatial Radius"] = mSpatialRadius;
//    ret["Color Radius"] = mColorRadius;
//    return ret;
//    }

//void PictureSmoothingTransform::setParamValue(const QString& aName, const QVariant& aValue)
//    {
//    if (aName == "Spatial Radius")
//        mSpatialRadius = aValue.toDouble();
//    else if (aName == "Color Radius")
//        mColorRadius = aValue.toDouble();
//    }

void PictureSmoothingTransform::process()
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

                cvPyrMeanShiftFiltering(srcImg, mPictureFrame, mSpatialRadius, mColorRadius);

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
