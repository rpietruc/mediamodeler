#ifndef POTRACEFRAMES_H
#define POTRACEFRAMES_H

#include "framebase.h"
#include "qubeframe.h"
#include <potracelib.h>

namespace media {

class PotraceImageFrame : public FrameBase
    {
public:
    enum { Width, Height, Dimensions };

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

class PotracePathFrame : public QubeFrame
    {
public:
    enum { XAxis, YAxis, MaxAxis };
    enum { StartPoint = 0, FirstPoint, SecondPoint, EndPoint, PathSize };
    enum { Index = PlaneNo, Point = VectorNo, Axis = ParamNo };

    explicit PotracePathFrame();
    ~PotracePathFrame();

    virtual void resize(const int *aSize);
    void resizeAndCopy(const potrace_path_t* aPath);

private:
    void resize(const potrace_path_t* aPath);
    void copy(const potrace_path_t* aPath);
    void setCurve(int aIndex, const potrace_dpoint_t (&aPoints)[PathSize]);
    };

} // namespace media

#endif // POTRACEFRAMES_H
