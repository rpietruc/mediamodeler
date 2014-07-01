#include "potracefiledestination.h"
#include "potraceframes.h"
#include <QTextStream>
#include <QSize>

namespace media {

PotraceFileDestination::PotraceFileDestination(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("fileName", "output.eps");
    }

bool isLine(const potrace_dpoint_t (&aPoints)[PotracePathFrame::PathSize])
    {
    return (aPoints[PotracePathFrame::FirstPoint].x == 0)
        && (aPoints[PotracePathFrame::FirstPoint].y == 0)
        && (aPoints[PotracePathFrame::SecondPoint].x == 0)
        && (aPoints[PotracePathFrame::SecondPoint].y == 0);
    }

void printLine(QTextStream& out, const potrace_dpoint_t (&aPoints)[PotracePathFrame::PathSize])
    {
    out << aPoints[PotracePathFrame::StartPoint].x << " " << aPoints[PotracePathFrame::StartPoint].y << " moveto" << endl;
    out << aPoints[PotracePathFrame::EndPoint].x << " " << aPoints[PotracePathFrame::EndPoint].y << " lineto" << endl;
    }

void printCurve(QTextStream& out, const potrace_dpoint_t (&aPoints)[PotracePathFrame::PathSize])
    {
    out << aPoints[PotracePathFrame::StartPoint].x << " " << aPoints[PotracePathFrame::StartPoint].y << " moveto" << endl;
    out << aPoints[PotracePathFrame::FirstPoint].x << " " << aPoints[PotracePathFrame::FirstPoint].y << " "
        << aPoints[PotracePathFrame::SecondPoint].x << " " << aPoints[PotracePathFrame::SecondPoint].y << " "
        << aPoints[PotracePathFrame::EndPoint].x << " " << aPoints[PotracePathFrame::EndPoint].y << " curveto" << endl;
    }

/** output vector data, e.g. as a rudimentary EPS file
 */
void saveEPS(QFile& aFile, const QSize& aSize, const PotracePathFrame& aPathFrame)
    {
    QTextStream out(&aFile);
    out << "\%!PS-Adobe-3.0 EPSF-3.0" << endl;
    out << "\%\%BoundingBox: 0 0 " << aSize.width() << " " << aSize.height() << endl;
    out << "gsave" << endl;

    int point[PotracePathFrame::Dimensions] = {0, 0, 0};
    for (point[PotracePathFrame::Index] = 0;
         point[PotracePathFrame::Index] < aPathFrame.getDimensionT(PotracePathFrame::Index).mResolution;
       ++point[PotracePathFrame::Index])
        {
        potrace_dpoint_t points[PotracePathFrame::PathSize];
        Q_ASSERT(aPathFrame.getDimensionT(PotracePathFrame::Point).mResolution == PotracePathFrame::PathSize);
        for (point[PotracePathFrame::Point] = 0;
             point[PotracePathFrame::Point] < PotracePathFrame::PathSize;
           ++point[PotracePathFrame::Point])
            {
            point[PotracePathFrame::Axis] = PotracePathFrame::XAxis;
            points[point[PotracePathFrame::Point]].x = aPathFrame.getSampleT(point);
            point[PotracePathFrame::Axis] = PotracePathFrame::YAxis;
            points[point[PotracePathFrame::Point]].y = aPathFrame.getSampleT(point);
            }

        if (isLine(points))
            printLine(out, points);
        else
            printCurve(out, points);
        out << "stroke" << endl;
        }
    out << "grestore" << endl;
    out << "\%EOF" << endl;
    }

void PotraceFileDestination::process()
    {
    QSize size(0, 0);
    PotracePathFrame pathFrame;

    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (pathFrame.isCopyable(*frame))
                pathFrame.resizeAndCopyFrame(*frame);

            else if (PotraceImageFrame().isCopyable(*frame))
                size = QSize(frame->getDimensionT(PotraceImageFrame::Width).mResolution,
                             frame->getDimensionT(PotraceImageFrame::Height).mResolution);
            }
    if ((size.width() > 0) && (size.height() > 0) && !pathFrame.isEmpty())
        {
        QFile file(property("fileName").toString());
        if (file.open(QIODevice::ReadWrite | QIODevice::Text))
           {
            saveEPS(file, size, pathFrame);
            emit framesProcessed();
            }
        }
    }

} // namespace media
