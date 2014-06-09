#include "exceptions.h"
#include <QDebug>

using namespace media;

void Exc::throwExcCode(int aExceptionCode, const char* aFile, int aLine)
    {
    qWarning() << "Exception about to be thrown, code: " << aExceptionCode << ", file: " << aFile << ", line: " << aLine;
    throw Exc(aExceptionCode);
    }

void Exc::throwExcCodeIfFalse(bool aCondition, int aExceptionCode, const char* aFile, int aLine)
    {
    if (!aCondition)
        throwExcCode(aExceptionCode, aFile, aLine);
    }
