#include "potracetracetransform.h"
#include "potraceframes.h"
#include <QTextStream>

namespace media {

PotraceTraceTransform::PotraceTraceTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("fileName", "output.eps");
    }

/** output vector data, e.g. as a rudimentary EPS file
 */
void saveEPS(QFile& aFile, int aWidth, int aHeight, const potrace_path_t* aPath)
    {
    QTextStream out(&aFile);
    out << "\%!PS-Adobe-3.0 EPSF-3.0" << endl;
    out << "\%\%BoundingBox: 0 0 " << aWidth << " " << aHeight << endl;
    out << "gsave" << endl;

    // draw each curve
    potrace_dpoint_t (*c)[3];
    while (aPath != NULL)
        {
        int n = aPath->curve.n;
        int *tag = aPath->curve.tag;
        c = aPath->curve.c;
        out << c[n-1][2].x << " " << c[n-1][2].y << " moveto" << endl;
        for (int i = 0; i < n; ++i)
            {
            switch (tag[i])
                {
            case POTRACE_CORNER:
                out << c[i][1].x << " " << c[i][1].y << " lineto" << endl;
                out << c[i][2].x << " " << c[i][2].y << " lineto" << endl;
                break;
            case POTRACE_CURVETO:
                out << c[i][0].x << " " << c[i][0].y << " " << c[i][1].x << " " << c[i][1].y << " " << c[i][2].x << " " << c[i][2].y << " curveto" << endl;
                break;
                }
            }
        // at the end of a group of a positive path and its negative children, fill.
        if (aPath->next == NULL || aPath->next->sign == '+')
            out << "0 setgray fill" << endl;
        aPath = aPath->next;
        }
    out << "grestore" << endl;
    out << "\%EOF" << endl;
    }

void PotraceTraceTransform::process()
    {
    PotraceImageFrame potraceImageFrame;
    foreach (const ElementBase *source, mSourceElementsReadySet)
        for (int i = 0; i < source->getFramesNo(); ++i)
            {
            const FrameBase *frame = source->getFrame(i);
            if (potraceImageFrame.isCopyable(*frame))
                potraceImageFrame.resizeAndCopyFrame(*frame);
            }
    if (!potraceImageFrame.isEmpty())
        {
        potrace_param_t *param = potrace_param_default();
        if (param)
            {
            param->turdsize = 0;
            potrace_state_t *st = potrace_trace(param, potraceImageFrame);
            if (st && st->status == POTRACE_STATUS_OK)
                {
                QFile file(property("fileName").toString());
                if (file.open(QIODevice::ReadWrite | QIODevice::Text))
                    {
                    saveEPS(file, potraceImageFrame.operator potrace_bitmap_t*()->w, potraceImageFrame.operator potrace_bitmap_t*()->h, st->plist);
                    emit framesProcessed();
                    }
                }
            }
        }
    }
} // namespace media
