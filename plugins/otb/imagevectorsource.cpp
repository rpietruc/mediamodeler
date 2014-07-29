#include "imagevectorsource.h"
#include <otbImageFileReader.h>

#include <QDebug>

//using namespace otb;
//using namespace itk;
using namespace std;

namespace media {

ImageVectorSource::ImageVectorSource(ElementFactory *aFactory, const QString &aObjectName) :
    FileListSource(aFactory, aObjectName)
    {
    }

void ImageVectorSource::process()
    {
    forever
        {
        QString filename = getNextFilePath();

        if (filename.isEmpty())
            {
            mImageFrame.clear();
            emit processingCompleted();
            break; //no more files
            }
        //else

        typedef otb::ImageFileReader<VectorOtbFrame::ImageType> ReaderType;
        ReaderType::Pointer reader = ReaderType::New();
        reader->SetFileName(filename.toStdString());
        reader->GenerateOutputInformation();
        if (reader->GetOutput()->GetNumberOfComponentsPerPixel() != this->mImageFrame.getDimensionT(VectorOtbFrame::Channels).mResolution)
            continue; //unsupported file type
        //else

        try
            {
            reader->Update();
            }
        catch (itk::ExceptionObject & exp)
            {  
            qWarning() << "ITK::Exception catched : " << exp.what();
            }

        mImageFrame = reader->GetOutput();
        mImageFrame.setSourceName(filename);
        emit framesReady();
        break; //file read
        }
    }

} // namespace media
