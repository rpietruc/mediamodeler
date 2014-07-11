#ifndef FRAME_H
#define FRAME_H

#include "globaldefs.h"
#include <QVector>
#include <QString>
#include <numeric>

namespace media {

class MEDIAMODELSHARED_EXPORT FrameBaseInterface
    {
public:
    virtual qreal getSampleT(const int *aPoint) const = 0;
    virtual void setSampleT(const int *aPoint, qreal aValue) = 0;
    virtual void resize(const int *aSize) = 0;
    };

class MEDIAMODELSHARED_EXPORT FrameBase : public FrameBaseInterface
    {
public:
    struct Dimension
        {
        qreal mStartLocation;
        qreal mDelta;
        int mResolution;

        bool operator==(const Dimension& aDimension) const;
        bool operator!=(const Dimension& aDimension) const { return !this->operator ==(aDimension); }
        const Dimension& operator=(const Dimension& aDimension);
        };

    explicit FrameBase(int aDimensions);
    virtual ~FrameBase() {}

    int getMaxDimension() const { return mDimensions.size(); }
    const Dimension& getDimensionT(int aIndex) const;

    QString getSourceName() const { return mSourceName; }
    void setSourceName(const QString& aSourceName) { mSourceName = aSourceName; }

    bool isEmpty() const;
    virtual bool inRange(const int *aPoint) const;

    /**
     * @brief isCopyable checks if parameter frame can be copied to this frame
     * @param aFrame
     * @return true if capy is possible
     */
    virtual bool isCopyable(const FrameBase& aFrame) const
        {
        return !aFrame.isEmpty()
            && (getMaxDimension() == aFrame.getMaxDimension());
        }

    /**
     * @brief resizeAndCopyFrame copies parameter frame data to this frame
     * @param aFrame
     * @return false if not copyable
     */
    virtual bool resizeAndCopyFrame(const FrameBase& aFrame);

protected:
    /**
     * @brief incrementPoint increments point starting from last dimension
     * @param aPoint
     * @return position of incremented dimension or -1 if range overflow
     */
    int incrementPoint(int *aPoint);

    /**
     * @brief zeroPoint sets point to zero in all frame dimensions
     * @param aPoint
     */
    void zeroPoint(int *aPoint);

protected:
    QString mSourceName;
    QVector<Dimension> mDimensions;
    };

class MEDIAMODELSHARED_EXPORT MonoChannelFrame : public FrameBase
    {
public:
    explicit MonoChannelFrame(int aDimensions) : FrameBase(aDimensions) {}
    virtual qreal compressChannel(const QVector<qreal>& aValues);
    virtual bool isCopyable(const FrameBase& aFrame) const
        {
        return !aFrame.isEmpty()
            && ((getMaxDimension() == aFrame.getMaxDimension()) ||
                (getMaxDimension() == (aFrame.getMaxDimension() - 1)));
        }

    virtual bool resizeAndCopyFrame(const FrameBase& aFrame);
    };

class MEDIAMODELSHARED_EXPORT MultiChannelFrame : public FrameBase
    {
public:
    explicit MultiChannelFrame(int aDimensions) : FrameBase(aDimensions) {}
    virtual bool isCopyable(const FrameBase& aFrame) const
        {
        return !aFrame.isEmpty()
            && ((getMaxDimension() == aFrame.getMaxDimension()) ||
                (getMaxDimension() == (aFrame.getMaxDimension() + 1)));
        }

    virtual bool resizeAndCopyFrame(const FrameBase& aFrame);
    };

} // namespace media

#endif // FRAME_H
