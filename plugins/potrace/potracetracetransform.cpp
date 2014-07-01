#include "potracetracetransform.h"

namespace media {

PotraceTraceTransform::PotraceTraceTransform(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
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
                mPathFrame.resizeAndCopy(st->plist);
                emit framesReady();
                }
            potrace_state_free(st);
            potrace_param_free(param);
            }
        }
    }
} // namespace media
