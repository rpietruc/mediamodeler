#ifndef SOUNDLINEARPREDICTIONFILEDESTINATION_H
#define SOUNDLINEARPREDICTIONFILEDESTINATION_H

#include "destinationbase.h"
#include <QString>
#include <QtCore/qglobal.h>
#include <QFile>
#include <QFileDialog>

#if defined(SOUNDLINEARPREDICTIONFILEDESTINATION_LIBRARY)
#  define SOUNDLINEARPREDICTIONFILEDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SOUNDLINEARPREDICTIONFILEDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class SOUNDLINEARPREDICTIONFILEDESTINATIONSHARED_EXPORT SoundLinearPredictionFileDestination : public DestinationBase
    {
    Q_OBJECT

public:
    SoundLinearPredictionFileDestination(DestinationFactory *aFactory, const QString &aObjectName);
    virtual ~SoundLinearPredictionFileDestination();

public slots:
    virtual void initSettings(const QString& aSettings);
    virtual void showSettingsDialog();

private slots:
    void open(const QString &aPath);

private:
    virtual bool process();

    QFileDialog mFileDialog;
    QFile mFile;
    };

class SoundLinearPredictionFileDestinationFactory : public DestinationFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::DestinationFactory)

protected:
    inline const char* getDestinationName() const { return "SoundLinearPredictionFileDestination"; }
    inline DestinationBase* createDestination(const QString &aObjectName) { return new SoundLinearPredictionFileDestination(this, aObjectName); }
    };

} // namespace media

#endif // SOUNDLINEARPREDICTIONFILEDESTINATION_H
