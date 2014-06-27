#include "potraceframes.h"
#include "exceptions.h"

/* macros for writing individual bitmap pixels */
//#define BM_WORDBITS (8*BM_WORDSIZE)
//#define BM_HIBIT (((potrace_word)1)<<(BM_WORDBITS-1))
//#define bm_scanline(bm, y) ((bm)->map + (y)*(bm)->dy)
//#define bm_index(bm, x, y) (&bm_scanline(bm, y)[(x)/BM_WORDBITS])
//#define bm_mask(x) (BM_HIBIT >> ((x) & (BM_WORDBITS-1)))
//#define bm_range(x, a) ((int)(x) >= 0 && (int)(x) < (a))
//#define bm_safe(bm, x, y) (bm_range(x, (bm)->w) && bm_range(y, (bm)->h))
//#define BM_USET(bm, x, y) (*bm_index(bm, x, y) |= bm_mask(x))
//#define BM_UCLR(bm, x, y) (*bm_index(bm, x, y) &= ~bm_mask(x))
//#define BM_UPUT(bm, x, y, b) ((b) ? BM_USET(bm, x, y) : BM_UCLR(bm, x, y))
//#define BM_PUT(bm, x, y, b) (bm_safe(bm, x, y) ? BM_UPUT(bm, x, y, b) : 0)

static int bm_size(potrace_bitmap_t *bm) { return bm->dy * bm->h * (int)sizeof(potrace_word); }
static int bm_wordbits() { return 8 * (int)sizeof(potrace_word); }

/* return new un-initialized bitmap. NULL with errno on error */
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
    mDimensions[Channels].mResolution = 1;
    mDimensions[Width].mDelta = mDimensions[Height].mDelta = mDimensions[Channels].mDelta = 1;
    mDimensions[Width].mResolution = mDimensions[Height].mResolution = 0;
    }

PotraceImageFrame::~PotraceImageFrame()
    {
    }

qreal PotraceImageFrame::getSampleT(const int *aPoint) const
    {
    return 0;
    }

void PotraceImageFrame::setSampleT(const int *aPoint, qreal aValue)
    {
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
    }

void PotraceImageFrame::clear()
    {
    Q_ASSERT(mBitmap);
    memset(mBitmap->map, 0, bm_size(mBitmap));
    }

} // namespace media
