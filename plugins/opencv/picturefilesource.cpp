#include "picturefilesource.h"

namespace media {

PictureFileSource::PictureFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    FileListSource(aFactory, aObjectName)
    {
    }

void PictureFileSource::process()
    {
    forever
        {
        QString filename = getNextFilePath();

        if (filename.isEmpty())
            {
            mRGBImg.release();
            emit processingCompleted();
            break; //no more files
            }
        //else

        IplImage* img = cvLoadImage(qPrintable(filename), CV_LOAD_IMAGE_COLOR);
        if (!img)
            continue; //file cannot be read
        //else

        mRGBImg = *img;
        cvReleaseImage(&img);
        mRGBImg.setSourceName(filename);
        emit framesReady();
        break; //file read
        }
    }

} // namespace media
