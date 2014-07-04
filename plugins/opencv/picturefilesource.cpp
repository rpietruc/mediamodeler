#include "picturefilesource.h"
#include <QDynamicPropertyChangeEvent>

namespace media {

PictureFileSource::PictureFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mNextFileIndex(0)
    {
    setProperty("fileList", QStringList());
    }

bool PictureFileSource::event(QEvent *aEvent)
    {
    if (aEvent->type() == QEvent::DynamicPropertyChange)
        {
        QDynamicPropertyChangeEvent *event = (QDynamicPropertyChangeEvent*)aEvent;
        if (QString(event->propertyName().constData()) == "fileList")
            {
            mPathList.clear();
            foreach (QString path, property("fileList").toStringList())
                {
                QFileInfo fileInfo(path);
                mPathList.append(fileInfo.filePath());
                }
            event->accept();
            return true;
            }
        }
    return ElementBase::event(aEvent);
    }

void PictureFileSource::process()
    {
    //if (mNextFileIndex >= mPathList.size())
    //    mRGBImg.releaseImage();

    IplImage* img = NULL;
    for (; !img && (mNextFileIndex < mPathList.size()); ++mNextFileIndex)
        img = cvLoadImage(qPrintable(mPathList.at(mNextFileIndex)), CV_LOAD_IMAGE_COLOR);

    if (img)
        {
        mRGBImg = *img;
        cvReleaseImage(&img);
        mRGBImg.setSourceName(mPathList.at(mNextFileIndex - 1));
        emit framesReady();
        }
    else //if (mNextFileIndex >= mPathList.size())
        {
        mRGBImg.release();
        mNextFileIndex = 0;
        emit processingCompleted();
        }
    }

} // namespace media
