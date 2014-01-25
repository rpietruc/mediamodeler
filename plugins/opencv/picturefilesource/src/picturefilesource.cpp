#include "picturefilesource.h"

namespace media {

PictureFileSource::PictureFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mNextFileIndex(0)
    {
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

void PictureFileSource::setFileList(QStringList aFileList)
    {
    mPathList.clear();
    foreach (QString path, aFileList)
        {
        QFileInfo fileInfo(path);
        mPathList.append(fileInfo.filePath());
        }
    emit fileListChanged();
    }
} // namespace media
