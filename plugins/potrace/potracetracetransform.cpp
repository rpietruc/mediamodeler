#include "potracetracetransform.h"

namespace media {

PotraceTraceTransform::PotraceTraceTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    potrace_param_t *param = potrace_param_default();
    setProperty("turdsize", param ? param->turdsize : 0); // area of largest path to be ignored
    setProperty("turnpolicy", param ? param->turnpolicy : 0); // resolves ambiguous turns in path decomposition
    setProperty("alphamax", param ? param->alphamax : 0); // corner threshold
    setProperty("opticurve", param ? param->opticurve : 0); // use curve optimization?
    setProperty("opttolerance", param ? param->opttolerance : 0); // curve optimization tolerance
    potrace_param_free(param);
    }

/** callback fn
  */
void printProgress(double progress, void *privdata)
    {
    PotraceTraceTransform* thiz = reinterpret_cast<PotraceTraceTransform*>(privdata);
    if (thiz)
        thiz->logProgress(progress);
    }

void PotraceTraceTransform::logProgress(double progress)
    {
    emit logMessage(Qt::lightGray, QString("progress: %1").arg((int)(progress*100)));
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
            param->turdsize = property("turdsize").toInt();
            param->turnpolicy = property("turnpolicy").toInt();
            param->alphamax = property("alphamax").toDouble();
            param->opticurve = property("opticurve").toInt();
            param->opttolerance = property("opttolerance").toDouble();

            // structure to hold progress bar callback data
            param->progress.callback = printProgress;
            param->progress.data = this; // callback function's private data
            param->progress.min = 0.0;
            param->progress.max = 1.0; // desired range of progress, e.g. 0.0 to 1.0
            param->progress.epsilon = 0.05; // granularity: can skip smaller increments

            potrace_state_t *st = potrace_trace(param, potraceImageFrame);
            if (st && st->status == POTRACE_STATUS_OK)
                {
                mPathFrame.resizeAndCopy(st->plist);
                emit framesReady();
                }
            potrace_state_free(st);
            potrace_param_free(param);
            }
        }
    }
} // namespace media
