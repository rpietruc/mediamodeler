#include "exceptions.h"
#include <QDebug>

using namespace media;

void Exc::throwExcCode(const char* aExceptionCode, const char* aFile, int aLine)
    {
    qWarning() << "Exception about to be thrown, code: " << aExceptionCode << ", file: " << aFile << ", line: " << aLine;
    throw Exc(aExceptionCode);
    }

void Exc::throwExcCode(const std::string& aExceptionCode, const char* aFile, int aLine)
    {
    qWarning() << "Exception about to be thrown, code: " << QString::fromStdString(aExceptionCode) << ", file: " << aFile << ", line: " << aLine;
    throw Exc(aExceptionCode);
    }

void Exc::throwExcCode(const QString& aExceptionCode, const char* aFile, int aLine)
    {
    qWarning() << "Exception about to be thrown, code: " << aExceptionCode << ", file: " << aFile << ", line: " << aLine;
    throw Exc(aExceptionCode);
    }
