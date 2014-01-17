#include "picturefilesource.h"

namespace media {

PictureFileSource::PictureFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mNextFileIndex(0)
    {
    }

ElementBase::ParamList PictureFileSource::getParams()
    {
    ParamList ret;
    ret["Files"] =  mPathList;
    return ret;
    }

void PictureFileSource::setParamValue(const QString& aName, const QVariant& aValue)
    {
    Q_UNUSED(aName);
    QStringList pathList = aValue.toStringList();
    mPathList.clear();
    foreach (QString path, pathList)
        {
        QFileInfo fileInfo(path);
        mPathList.append(fileInfo.filePath());
        }
    }

void PictureFileSource::process()
    {
    //if (mNextFileIndex >= mPathList.size())
    //    mPictureFrame.releaseImage();

    IplImage* img = NULL;
    for (; !img && (mNextFileIndex < mPathList.size()); ++mNextFileIndex)
        img = cvLoadImage(qPrintable(mPathList.at(mNextFileIndex)), CV_LOAD_IMAGE_COLOR);

    if (img)
        {
        mPictureFrame.resizeAndCopyImage(*img);
        cvReleaseImage(&img);
        mPictureFrame.setSourceName(mPathList.at(mNextFileIndex - 1));
        emit framesReady();
        }
    else //if (mNextFileIndex >= mPathList.size())
        {
        mPictureFrame.release();
        mNextFileIndex = 0;
        emit processingCompleted();
        }
    }

} // namespace media
