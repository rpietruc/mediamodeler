#include "framebase.h"
#include "exceptions.h"
#include <vector>
#include <numeric>
#include <QScopedArrayPointer>

using namespace media;
using namespace std;

bool FrameBase::Dimension::operator==(const Dimension& aDimension) const
    {
    return (mStartLocation == aDimension.mStartLocation) &&
           (mDelta == aDimension.mDelta) &&
           (mResolution == aDimension.mResolution);
    }

const FrameBase::Dimension& FrameBase::Dimension::operator=(const Dimension& aDimension)
    {
    mDelta = aDimension.mDelta;
    mResolution = aDimension.mResolution;
    mStartLocation = aDimension.mStartLocation;
    return *this;
    }

FrameBase::FrameBase(int aDimensions)
    {
    Dimension dim = {0, 0, 0};
    for (int i = 0; i < aDimensions; ++i)
        mDimensions.push_back(dim);
    }

const FrameBase::Dimension& FrameBase::getDimensionT(int aIndex) const
    {
    Q_ASSERT(aIndex < mDimensions.size());
    return mDimensions.at(aIndex);
    }

bool FrameBase::isEmpty() const
    {
    for (int i = 0; i < getMaxDimension(); ++i)
        if (getDimensionT(i).mResolution <= 0)
            return true;
    return false;
    }

bool FrameBase::inRange(const int *aPoint) const
    {
    for (int i = 0; i < getMaxDimension(); ++i)
        if ((aPoint[i] < 0) || (aPoint[i] >= getDimensionT(i).mResolution))
            return false;
    return true;
    }

void FrameBase::zeroPoint(int *aPoint)
    {
    for (int i = 0; i < getMaxDimension(); ++i)
        aPoint[i] = 0;
    }

int FrameBase::incrementPoint(int *aPoint)
    {
    int ret = -1;
    for (int i = (getMaxDimension() - 1); i >= 0 ; --i)
        if (aPoint[i] < (getDimensionT(i).mResolution - 1))
            {
            ++aPoint[i];
            for (int j = (i + 1); j < getMaxDimension(); ++j)
                aPoint[j] = 0;
            ret = i;
            break;
            }
    if (ret < 0)
        for (int i = 0; i < getMaxDimension(); ++i)
            aPoint[i] = 0;
    return ret;
    }

bool FrameBase::resizeAndCopyFrame(const FrameBase& aFrame)
    {
    if (!FrameBase::isCopyable(aFrame))
        return false;

    QScopedArrayPointer<int> size(new int[getMaxDimension()]);
    for (int i = 0; i < getMaxDimension(); ++i)
        size[i] = aFrame.getDimensionT(i).mResolution;
    resize(size.data());

    QScopedArrayPointer<int> point(new int[getMaxDimension()]);
    zeroPoint(point.data());
    do
        setSampleT(point.data(), aFrame.getSampleT(point.data()));
        while (incrementPoint(point.data()) >= 0);

    return true;
    }

struct Mean
    {
    unsigned int n;
    Mean(unsigned int n) : n(n) {}
    qreal operator() (qreal sum, qreal x) { return sum + x/n; }
    };

qreal MonoChannelFrame::compressChannel(const QVector<qreal>& aValues)
    {
    Mean mean(aValues.size());
    std::vector<qreal> values = aValues.toStdVector();
    return accumulate(values.begin(), values.end(), 0.0, mean);
    }

bool MonoChannelFrame::resizeAndCopyFrame(const FrameBase& aFrame)
    {
    if (!MonoChannelFrame::isCopyable(aFrame))
        return false;

    if (FrameBase::isCopyable(aFrame))
        return FrameBase::resizeAndCopyFrame(aFrame);

    QScopedArrayPointer<int> size(new int[getMaxDimension()]);
    for (int i = 0; i < getMaxDimension(); ++i)
        size[i] = aFrame.getDimensionT(i).mResolution;
    resize(size.data());

    QScopedArrayPointer<int> point(new int[getMaxDimension() + 1]);
    zeroPoint(point.data());
    do
        {
        QVector<qreal> values;
        for (point[getMaxDimension()] = 0; point[getMaxDimension()] < aFrame.getDimensionT(getMaxDimension()).mResolution; ++point[getMaxDimension()])
            values.push_back(aFrame.getSampleT(point.data()));
        setSampleT(point.data(), compressChannel(values));
        }
        while (incrementPoint(point.data()) >= 0);

    return true;
    }

bool MultiChannelFrame::resizeAndCopyFrame(const FrameBase& aFrame)
    {
    if (!MultiChannelFrame::isCopyable(aFrame))
        return false;

    if (FrameBase::isCopyable(aFrame))
        return FrameBase::resizeAndCopyFrame(aFrame);

    QScopedArrayPointer<int> size(new int[getMaxDimension()]);
    for (int i = 0; i < (getMaxDimension() - 1); ++i)
        size[i] = aFrame.getDimensionT(i).mResolution;
    size[getMaxDimension() - 1] = mDimensions[getMaxDimension() - 1].mResolution;
    resize(size.data());

    QScopedArrayPointer<int> point(new int[getMaxDimension()]);
    zeroPoint(point.data());
    do
        setSampleT(point.data(), aFrame.getSampleT(point.data()));
        while (incrementPoint(point.data()) >= 0);

    return true;
    }
