#ifndef MEDIA_CONTOURAPPROXIMATIONTRANSFORM_H
#define MEDIA_CONTOURAPPROXIMATIONTRANSFORM_H

#include "elementbase.h"
#include <QtCore/qglobal.h>
#include <ellipseframe.h>
#include <linesegmentframe.h>

#if defined(contourapproximationtransform_EXPORTS)
#  define CONTOURAPPROXIMATIONTRANSFORMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONTOURAPPROXIMATIONTRANSFORMSHARED_EXPORT Q_DECL_IMPORT
#endif

namespace media {

class CONTOURAPPROXIMATIONTRANSFORMSHARED_EXPORT ContourApproximationTransform : public ElementBase
    {
    Q_OBJECT

public:
    explicit ContourApproximationTransform(ElementFactory *aFactory, const QString &aObjectName);

    int getFramesNo() const { return mEllipseFrames.size() + mLineSegmentFrames.size(); }
    const FrameBase *getFrame(int aIndex) const
        {
        if (aIndex < mEllipseFrames.size())
            return &mEllipseFrames.at(aIndex);
        /*else*/ if (aIndex < getFramesNo())
            return &mLineSegmentFrames.at(aIndex - mEllipseFrames.size());
        /*else*/ return 0;
        }

private:
    void process();

private:
    QVector<EllipseFrame> mEllipseFrames;
    QVector<LineSegmentFrame> mLineSegmentFrames;
    };

class CONTOURAPPROXIMATIONTRANSFORMSHARED_EXPORT ContourApproximationTransformFactory : public ElementFactory
    {
    Q_OBJECT
    Q_INTERFACES(media::ElementFactory)
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCustomPlugin")

protected:
    const char* getElementName() const { return "ContourApproximationTransform"; }
    ElementBase* createElement(const QString &aObjectName) { return new ContourApproximationTransform(this, aObjectName); }
    };

} // namespace media

#endif // MEDIA_CONTOURAPPROXIMATIONTRANSFORM_H
