#include "picturefilesource.h"
#include <QDynamicPropertyChangeEvent>

namespace media {

PictureFileSource::PictureFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mNextFileIndex(0)
    {
    setProperty("fileList", QStringList());
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

bool PictureFileSource::event(QEvent *aEvent)
    {
    if (aEvent->type() == QEvent::DynamicPropertyChange)
        {
        QDynamicPropertyChangeEvent *event = (QDynamicPropertyChangeEvent*)aEvent;
        if (QString(event->propertyName().constData()) == "fileList")
            {
            // special tab handling here
            mPathList.clear();
            foreach (QString path, property("fileList").toStringList())
                {
                QFileInfo fileInfo(path);
                mPathList.append(fileInfo.filePath());
                }
            event->accept();
            return TRUE;
            }
        }
    return ElementBase::event(aEvent);
    }

} // namespace media
