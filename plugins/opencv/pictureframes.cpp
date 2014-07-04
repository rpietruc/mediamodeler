#include "pictureframes.h"
#include <opencv/cv.h>
#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace media {

PointsFrame::PointsFrame() :
    FrameBase(Dimensions)
    {
    mDimensions[Axis].mDelta = 1;
    mDimensions[Axis].mResolution = MaxAxis;
    }

qreal PointsFrame::getSampleT(const int *aPoint) const
    {
    Q_ASSERT(aPoint[Axis] < MaxAxis);
    Q_ASSERT(aPoint[Index] < (int)mPoints.size());
    return (aPoint[Axis] == XAxis ? mPoints[aPoint[Index]].x : mPoints[aPoint[Index]].y);
    }

void PointsFrame::setSampleT(const int *aPoint, qreal aValue)
    {
    Q_ASSERT(aPoint[Axis] < MaxAxis);
    Q_ASSERT(aPoint[Index] < (int)mPoints.size());
    if (aPoint[Axis] == XAxis)
        mPoints[aPoint[Index]].x = aValue;
    else
        mPoints[aPoint[Index]].y = aValue;
    }

bool PointsFrame::readFromFile(const QString& aFileName)
    {
    QFile inFile(aFileName);
    if (inFile.open(QIODevice::Text|QIODevice::ReadOnly))
        {
        QTextStream inStream(&inFile);
        CvPoint point;
        while (!inStream.atEnd())
            {
            inStream >> point.x >> point.y;
            mPoints.push_back(point);
            }
        return true;
        }
    return false;
    }

void PointsFrame::writeToFile(const QString& aFileName) const
    {
    QFile outFile(aFileName);
    if (outFile.open(QIODevice::Text|QIODevice::ReadWrite))
        {
        QTextStream outStream(&outFile);
        foreach (CvPoint point, mPoints)
            outStream << point.x << point.y;
        }
    }

} // namespace media
