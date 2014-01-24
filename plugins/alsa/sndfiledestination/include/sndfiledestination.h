#ifndef MEDIA_SNDFILEDESTINATION_H
#define MEDIA_SNDFILEDESTINATION_H

#include "elementbase.h"
#include <QString>
#include <sndfile.h>
#include <QtCore/qglobal.h>
#include "alsaframe.h"

#if defined(SNDFILEDESTINATION_LIBRARY)
#  define SNDFILEDESTINATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SNDFILEDESTINATIONSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class SNDFILEDESTINATIONSHARED_EXPORT SndFileDestination : public ElementBase
    {
    Q_OBJECT

public:
    explicit SndFileDestination(ElementFactory *aFactory, const QString &aObjectName);
    ~SndFileDestination();

    ParamList getParams() const;

public slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private slots:
    void write();

private:
    void process();
    bool open(SF_INFO &aFileInfo);
    void close();

private:
    SNDFILE *mSoundFile;
    AlsaFrame mAlsaFrame;
    QString mFileName;
    };

class SndFileDestinationFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "SndFileDestination"; }
    ElementBase* createElement(const QString &aObjectName) { return new SndFileDestination(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_SNDFILEDESTINATION_H
