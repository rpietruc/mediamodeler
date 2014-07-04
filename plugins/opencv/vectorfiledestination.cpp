#include "vectorfiledestination.h"
#include "ellipseframe.h"
#include "linesegmentframe.h"
#include "pictureframes.h"
#include <QtSvg/QSvgGenerator>
#include <QtGui/QPainter>

using namespace cv;

namespace media {

VectorFileDestination::VectorFileDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName),
    mFileIndex(0)
    {
    setProperty("fileName", "output.svg");
    }

qreal arcLength(qreal aStartAngle, qreal aEndAngle) { return aEndAngle - aStartAngle; }

void paintEllipticalArc(QPainter &aPainter, const RotatedRect& aRect, double aStartAngle, double aEndAngle)
    {
    QRectF rect(QPointF(0, 0), QSizeF(aRect.size.width, aRect.size.height));
    QPainterPath path;
    path.arcMoveTo(rect, aStartAngle/* + 90*/);
    path.arcTo(rect, aStartAngle/* + 90*/, arcLength(aStartAngle, aEndAngle));
    path.arcMoveTo(rect, aStartAngle);
    path.moveTo(0, 0);

    QMatrix mat;
    mat.translate(aRect.center.x, aRect.center.y);
    mat.rotate(aRect.angle);
    mat.translate(-aRect.size.width/2, -aRect.size.height/2);
    path = mat.map(path);

    aPainter.drawPath(path);
    }

void paintLineSegment(QPainter &aPainter, const QPointF& aPoint1, const QPointF& aPoint2)
    {
    QPainterPath path;
    path.moveTo(aPoint1);
    path.lineTo(aPoint2);
    aPainter.drawPath(path);
    }

void VectorFileDestination::process()
    {
    QSize size;

    QVector<EllipseFrame> ellipses;
    QVector<LineSegmentFrame> lineSegments;

    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (EllipseFrame().isCopyable(*frame))
                ellipses.push_back(EllipseFrame(*frame));

            else if (LineSegmentFrame().isCopyable(*frame))
                lineSegments.push_back(LineSegmentFrame(*frame));

            else if (PictureRGBFrame().isCopyable(*frame))
                size = QSize(frame->getDimensionT(PictureRGBFrame::Width).mResolution,
                             frame->getDimensionT(PictureRGBFrame::Height).mResolution);
            }
//    size = QSize(600, 450);

    QSvgGenerator generator;
    QString fileName(property("fileName").toString());
    if (mFileIndex > 0)
        fileName.replace(QString(".svg"), QString(".%1.svg").arg(mFileIndex));
    generator.setFileName(fileName);
    generator.setTitle(tr("SVG Generator Ellipse Drawing"));
    generator.setDescription(tr("An SVG drawing created by VectorFileDestination."));
    generator.setSize(size);
    generator.setViewBox(QRect(0, 0, size.width(), size.height()));

    QPainter painter;
    painter.begin(&generator);
    painter.setClipRect(QRect(0, 0, size.width(), size.height()));
    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::SolidLine);

    int ellipsesNo = 0;
    foreach (EllipseFrame frame, ellipses)
        for (int i = 0; i < frame.getDimensionT(EllipseFrame::VectorNo).mResolution; ++i, ++ellipsesNo)
            paintEllipticalArc(painter, frame.getRect(i), frame.getStartAngleT(i), frame.getEndAngle(i));

    int linesNo = 0;
    foreach (LineSegmentFrame frame, lineSegments)
        for (int i = 0; i < frame.getDimensionT(LineSegmentFrame::VectorNo).mResolution; ++i, ++linesNo)
            paintLineSegment(painter, frame.getStartPointT(i), frame.getEndPointT(i));

    emit logMessage(Qt::red, QString("total: %1 ellipses and %2 line segments").arg(ellipsesNo).arg(linesNo));

//    paintEllipticalArc(painter, RotatedRect(Point2f(200, 150), Size2f(400, 300), 30), 0, 360);
//    paintLineSegment(painter, QPointF(0, 0), QPointF(400, 300));

    ++mFileIndex;
    painter.end();
    emit framesProcessed();
    }

} // namespace media
