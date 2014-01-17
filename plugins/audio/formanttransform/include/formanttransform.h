#ifndef FORMANTTRANSFORM_H
#define FORMANTTRANSFORM_H

#include "elementbase.h"
#include "formantframe.h"
#include <QtCore/qglobal.h>

#if defined(FORMANTTRANSFORM_LIBRARY)
#  define FORMANTTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FORMANTTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class FORMANTTRANSFORMSHARED_EXPORT FormantTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit FormantTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return 1; }
    const FrameBase *getFrame(int) const { return &mFormantFrame; }

    ParamList getParams();

public slots:
    void setParamValue(const QString& aName, const QVariant& aValue);

private slots:
    void setMaxFormants(int aFormantsNo);

private:
    void process();

private:
    FormantFrame mFormantFrame;
    };

class FORMANTTRANSFORMSHARED_EXPORT FormantTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "FormantTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new FormantTransform(this, aObjectName); }
    };

} // namespace media

#endif // FORMANTTRANSFORM_H
