#include "potraceframes.h"
#include "exceptions.h"

// macros for writing individual bitmap pixels
#define BM_WORDSIZE ((int)sizeof(potrace_word))
#define BM_WORDBITS (8*BM_WORDSIZE)
#define BM_HIBIT (((potrace_word)1)<<(BM_WORDBITS-1))
#define bm_scanline(bm, y) ((bm)->map + (y)*(bm)->dy)
#define bm_index(bm, x, y) (&bm_scanline(bm, y)[(x)/BM_WORDBITS])
#define bm_mask(x) (BM_HIBIT >> ((x) & (BM_WORDBITS-1)))
#define bm_range(x, a) ((int)(x) >= 0 && (int)(x) < (a))
#define bm_safe(bm, x, y) (bm_range(x, (bm)->w) && bm_range(y, (bm)->h))
#define BM_USET(bm, x, y) (*bm_index(bm, x, y) |= bm_mask(x))
#define BM_UCLR(bm, x, y) (*bm_index(bm, x, y) &= ~bm_mask(x))
#define BM_UPUT(bm, x, y, b) ((b) ? BM_USET(bm, x, y) : BM_UCLR(bm, x, y))
#define BM_PUT(bm, x, y, b) (bm_safe(bm, x, y) ? BM_UPUT(bm, x, y, b) : 0)

#define BM_UGET(bm, x, y) (*bm_index(bm, x, y) & bm_mask(x))
#define BM_UPICK(bm, x, y) (BM_UGET(bm, x, y) ? 1 : 0)
#define BM_PICK(bm, x, y) (bm_safe(bm, x, y) ? BM_UPICK(bm, x, y) : 0)

static int bm_size(potrace_bitmap_t *bm) { return bm->dy * bm->h * (int)sizeof(potrace_word); }
static int bm_wordbits() { return 8 * (int)sizeof(potrace_word); }

// return new un-initialized bitmap. NULL with errno on error
static potrace_bitmap_t *bm_new(int w, int h)
    {
    potrace_bitmap_t *bm;
    int dy = (w + bm_wordbits() - 1) / bm_wordbits();

    bm = (potrace_bitmap_t *) malloc(sizeof(potrace_bitmap_t));
    if (!bm)
        return NULL;

    bm->w = w;
    bm->h = h;
    bm->dy = dy;
    bm->map = (potrace_word *) malloc(bm_size(bm));
    if (!bm->map)
        {
        free(bm);
        return NULL;
        }
    return bm;
    }

/* free a bitmap */
static void bm_free(potrace_bitmap_t *bm)
    {
    if (bm != NULL)
        free(bm->map);
    free(bm);
    }

namespace media {

PotraceImageFrame::PotraceImageFrame() :
    FrameBase(Dimensions),
    mBitmap(NULL)
    {
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

PotraceImageFrame::~PotraceImageFrame()
    {
    bm_free(mBitmap);
    }

qreal PotraceImageFrame::getSampleT(const int *aPoint) const
    {
    return BM_PICK(mBitmap, aPoint[Width], aPoint[Height])*255;
    }

void PotraceImageFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    BM_PUT(mBitmap, aPoint[Width], aPoint[Height], aValue > 0);
    }

void PotraceImageFrame::resize(int aWidth, int aHeight)
    {
    if (mBitmap && ((aWidth != mBitmap->w) || (aHeight != mBitmap->h)))
        {
        bm_free(mBitmap);
        mBitmap = NULL;
        }
    if (mBitmap == NULL)
        mBitmap = bm_new(aWidth, aHeight);

    mDimensions[Width].mResolution = aWidth;
    mDimensions[Height].mResolution = aHeight;
    }

void PotraceImageFrame::clear()
    {
    Q_ASSERT(mBitmap);
    memset(mBitmap->map, 0, bm_size(mBitmap));
    }

PotracePathFrame::PotracePathFrame()
    {
    mDimensions[Index].mDelta = mDimensions[Point].mDelta = mDimensions[Axis].mDelta = 1;
    mDimensions[Index].mResolution = mDimensions[Point].mResolution = 0;
    mDimensions[Axis].mResolution = 2;
    }

PotracePathFrame::~PotracePathFrame()
    {
    }

void PotracePathFrame::resize(const int* aSize)
    {
    setMaxPlanes(aSize[PlaneNo]);

    Q_ASSERT(aSize[VectorNo] == PathSize);
    setMaxVectorsT(PathSize);

    Q_ASSERT(aSize[ParamNo] == MaxAxis);
    setVectorSizeT(MaxAxis);
    }

void PotracePathFrame::resizeAndCopy(const potrace_path_t* aPath)
    {
    resize(aPath);
    copy(aPath);
    }

void PotracePathFrame::resize(const potrace_path_t* aPath)
    {
    int pointNo = 0;
    while (aPath != NULL)
        {
        int n = aPath->curve.n;
        int *tag = aPath->curve.tag;
        for (int i = 0; i < n; ++i)
            {
            switch (tag[i])
                {
            case POTRACE_CORNER:
                pointNo += 2;
                break;

            case POTRACE_CURVETO:
                ++pointNo;
                break;
                }
            }
        aPath = aPath->next;
        }
    int aSize[] = {pointNo, PathSize, MaxAxis};
    resize(aSize);
    }

void PotracePathFrame::copy(const potrace_path_t* aPath)
    {
    potrace_dpoint_t points[PathSize];
    int pointIndex = 0;

    while (aPath != NULL)
        {
        int n = aPath->curve.n;
        int *tag = aPath->curve.tag;
        potrace_dpoint_t (*c)[3] = aPath->curve.c;

        points[StartPoint].x = c[n-1][2].x;
        points[StartPoint].y = c[n-1][2].y;

        for (int i = 0; i < n; ++i)
            {
            switch (tag[i])
                {
            case POTRACE_CORNER:

                points[FirstPoint].x = points[FirstPoint].y = points[SecondPoint].x = points[SecondPoint].y = 0;

                points[EndPoint].x = c[i][1].x;
                points[EndPoint].y = c[i][1].y;
                setCurve(pointIndex++, points);
                points[StartPoint].x = c[i][1].x;
                points[StartPoint].y = c[i][1].y;

                points[EndPoint].x = c[i][2].x;
                points[EndPoint].y = c[i][2].y;
                setCurve(pointIndex++, points);
                points[StartPoint].x = c[i][2].x;
                points[StartPoint].y = c[i][2].y;

                break;

            case POTRACE_CURVETO:
                points[FirstPoint].x = c[i][0].x;
                points[FirstPoint].y = c[i][0].y;
                points[SecondPoint].x = c[i][1].x;
                points[SecondPoint].y = c[i][1].y;
                points[EndPoint].x = c[i][2].x;
                points[EndPoint].y = c[i][2].y;
                setCurve(pointIndex++, points);
                points[StartPoint].x = c[i][2].x;
                points[StartPoint].y = c[i][2].y;
                break;
                }

            if (aPath->next == NULL || aPath->next->sign == '+')
                {
                //next stroke
                }
            }
        aPath = aPath->next;
        }
    }

void PotracePathFrame::setCurve(int aIndex, const potrace_dpoint_t (&aPoints)[PathSize])
    {
    int point[Dimensions] = {aIndex, 0, 0};
    for (point[Point] = 0; point[Point] < PathSize; ++point[Point])
        {
        point[Axis] = XAxis;
        setSampleT(point, aPoints[point[Point]].x);
        point[Axis] = YAxis;
        setSampleT(point, aPoints[point[Point]].y);
        }
    }

} // namespace media
