#ifndef FRAME_H
#define FRAME_H

#include "globaldefs.h"
#include <QVector>
#include <QSet>
#include <QString>
#include "exceptions.h"

namespace media {

class MEDIAMODELSHARED_EXPORT FrameBase
    {
public:
    struct Dimension
        {
        qreal mStartLocation;
        qreal mDelta;
        int mResolution;

        bool operator==(const Dimension& aDimension) const
            {
            return (mStartLocation == aDimension.mStartLocation) &&
                   (mDelta == aDimension.mDelta) &&
                   (mResolution == aDimension.mResolution);
            }

        bool operator!=(const Dimension& aDimension) const
            {
            return !this->operator ==(aDimension);
            }
        };

    explicit FrameBase(int aDimensions)
        {
        Dimension dim = {0, 0, 0};
        for (int i = 0; i < aDimensions; ++i)
            mDimensions.push_back(dim);
        }

    int getMaxDimension() const { return mDimensions.size(); }
    const Dimension& getDimensionT(int aIndex) const
        {
        Exc::throwExcCodeIfFalse(aIndex < mDimensions.size(), 0, __FILE__, __LINE__);
        return mDimensions.at(aIndex);
        }

    virtual qreal getSampleT(const int *aPoint) const = 0;

    QString getSourceName() const { return mSourceName; }
    void setSourceName(const QString& aSourceName) { mSourceName = aSourceName; }

    bool isEmpty() const
        {
        for (int i = 0; i < getMaxDimension(); ++i)
            if (getDimensionT(i).mResolution <= 0)
                return true;
        return false;
        }

protected:
    QString mSourceName;
    QVector<Dimension> mDimensions;
    };

} // namespace media

#endif // FRAME_H
