#ifndef MEDIAMODEL_EXCEPTIONS_H
#define MEDIAMODEL_EXCEPTIONS_H

#include "globaldefs.h"

namespace media
{
    //! Exc - simple utility tracing exceptions
    /*!
        This class provides a simple tracing for thrown exceptions.
        All traces are output into the standard log file.
    */
    class MEDIAMODELSHARED_EXPORT Exc
        {
    public:
        static void throwExcCode(int aExceptionCode, const char* aFile = 0, int aLine = 0);
        static void throwExcCodeIfFalse(bool aCondition, int aExceptionCode, const char* aFile = 0, int aLine = 0);

    public:
        inline Exc(int aExceptionCode) : mExceptionCode(aExceptionCode) {}
        int mExceptionCode;
        };

} //namespace media

#endif //MEDIAMODEL_EXCEPTIONS_H
