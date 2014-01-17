#include "soundlinearpredictionfiledestination.h"
#include "soundframes.h"
#include <QDebug>
#include <QtPlugin>
#include <QTextStream>

namespace media {

SoundLinearPredictionFileDestination::SoundLinearPredictionFileDestination(DestinationFactory *aFactory, const QString &aObjectName) :
    DestinationBase(aFactory, aObjectName)
    {
    QObject::connect(&mFileDialog, SIGNAL(fileSelected(QString)), this, SLOT(open(QString)));
    }

SoundLinearPredictionFileDestination::~SoundLinearPredictionFileDestination()
    {
    if (mFile.isOpen())
        mFile.close();
    }

void SoundLinearPredictionFileDestination::showSettingsDialog()
    {
    mFileDialog.show();
    }

void SoundLinearPredictionFileDestination::initSettings(const QString& aSettings)
    {
    open(aSettings);
    }

void SoundLinearPredictionFileDestination::open(const QString &aFileName)
    {
    if (mFile.isOpen())
        mFile.close();
    mFile.setFileName(aFileName);
    mFile.open(QIODevice::WriteOnly|QIODevice::Text);
    if (mFile.isOpen())
        emit settingsChanged(aFileName);
    }

bool SoundLinearPredictionFileDestination::process()
    {
    const FrameBase *frame = *mCurrentFramesReadySet.begin();

    Q_ASSERT(frame->getMaxDimension() == LPCFrame::Dimensions);
    QTextStream textStream(&mFile);
    int point[LPCFrame::Dimensions];
    for (point[LPCFrame::Time] = 0; point[LPCFrame::Time] < frame->getDimension(LPCFrame::Time).mResolution; ++point[LPCFrame::Time])
        {
        textStream << frame->getSourceName() << " " << frame->getDimension(LPCFrame::Time).mStartLocation << " ";
        for (point[LPCFrame::Taps] = 0; point[LPCFrame::Taps] < frame->getDimension(LPCFrame::Taps).mResolution; ++point[LPCFrame::Taps])
            textStream << frame->getSample(point) << " ";
        textStream << "\n";
        }
    return true;
    }

Q_EXPORT_PLUGIN2(PLUGINTARGETNAME, SoundLinearPredictionFileDestinationFactory);

} // namespace media
