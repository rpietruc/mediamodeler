#include "picturemorphologytransform.h"

namespace media {

PictureMorphologyTransform::PictureMorphologyTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mMorphologySize(1),
    mOperation(CV_MOP_GRADIENT)
    {
    mStructuringElement = cvCreateStructuringElementEx(mMorphologySize*2 + 1, mMorphologySize*2 + 1, mMorphologySize, mMorphologySize, CV_SHAPE_RECT, 0);
    }

PictureMorphologyTransform::~PictureMorphologyTransform()
    {
    cvReleaseStructuringElement(&mStructuringElement);
    }

ElementBase::ParamList PictureMorphologyTransform::getParams() const
    {
    ParamList ret;
    ret["Morphology Size"] = mMorphologySize;
    ret["Operation"] = mOperation;
    return ret;
    }

void PictureMorphologyTransform::setParamValue(const QString& aName, const QVariant& aValue)
    {
    if (aName == "Morphology Size")
        {
        if (mMorphologySize != aValue.toInt())
            {
            mMorphologySize = aValue.toInt();
            cvReleaseStructuringElement(&mStructuringElement);
            mStructuringElement = cvCreateStructuringElementEx(mMorphologySize*2 + 1, mMorphologySize*2 + 1, mMorphologySize, mMorphologySize, CV_SHAPE_RECT, 0);
            }
        }
    else if (aName == "Operation")
        mOperation = aValue.toInt();
    }

void PictureMorphologyTransform::process()
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

                cvMorphologyEx(srcImg, mPictureFrame, mTempFrame, mStructuringElement, mOperation, 1);

                emit framesReady();
                break;
                }
            }
    }

} // namespace media
