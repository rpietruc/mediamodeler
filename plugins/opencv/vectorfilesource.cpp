#include "vectorfilesource.h"
#include <QtSvg/QSvgRenderer>
#include <QtGui/QPixmap>
#include <QtGui/QPainter>

namespace media {

VectorFileSource::VectorFileSource(ElementFactory *aFactory, const QString &aObjectName) :
    FileListSource(aFactory, aObjectName)
    {
    }

void VectorFileSource::process()
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

        QSvgRenderer renderer(filename);
        QPixmap pm(renderer.defaultSize());
        pm.fill(Qt::black);
        QPainter painter(&pm);
        renderer.render(&painter, pm.rect());

        mRGBImg = pm.toImage();
        mRGBImg.setSourceName(filename);

        emit framesReady();
        break; //file read
        }
    }

} // namespace media
