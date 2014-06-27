#include "potracegeneratorsource.h"

namespace media {

PotraceGeneratorSource::PotraceGeneratorSource(ElementFactory *aFactory, const QString &aObjectName) :
    ElementBase(aFactory, aObjectName)
    {
    setProperty("width", 800);
    setProperty("height", 600);
    mImageFrame.setSourceName(objectName());
    }

void PotraceGeneratorSource::process()
    {
    }

} // namespace media
