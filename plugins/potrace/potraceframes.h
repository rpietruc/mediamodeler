#ifndef IMAGEFRAMES_H
#define IMAGEFRAMES_H

#include "framebase.h"
#include <potracelib.h>

namespace media {

class PotraceImageFrame : public FrameBase
    {
public:
    enum { Width, Height, Channels, Dimensions };

    explicit PotraceImageFrame();
    ~PotraceImageFrame();

    qreal getSampleT(const int *aPoint) const;
    void setSampleT(const int *aPoint, qreal aValue);

    operator potrace_bitmap_t*() const { return mBitmap; }

    void resize(const int* aSize) { resize(aSize[Width], aSize[Height]); }
    void resize(int aWidth, int aHeight);
    void clear();

private:
    potrace_bitmap_t *mBitmap;
    };

} // namespace media

#endif // IMAGEFRAMES_H
