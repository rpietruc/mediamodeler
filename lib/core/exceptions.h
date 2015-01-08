#ifndef MEDIAMODEL_EXCEPTIONS_H
#define MEDIAMODEL_EXCEPTIONS_H

#include "globaldefs.h"
#include <exception>
#include <QString>

#define ThrowExcCodeIfFalse(aCondition, aExceptionCode, aFile, aLine) { \
    if (!(aCondition)) \
        Exc::throwExcCode(aExceptionCode, aFile, aLine); \
        }

namespace media
{
    //! Exc - simple utility tracing exceptions
    /*!
        This class provides a simple tracing for thrown exceptions.
        All traces are output into the standard log file.
    */
    class MEDIAMODELSHARED_EXPORT Exc: public std::exception
        {
    public:
        static void throwExcCode(const char* aExceptionCode, const char* aFile = 0, int aLine = 0);
        static void throwExcCode(const std::string& aExceptionCode, const char* aFile = 0, int aLine = 0);
        static void throwExcCode(const QString& aExceptionCode, const char* aFile = 0, int aLine = 0);

        explicit Exc(const char* aMsg):
            mMsg(aMsg)
            {
            }

        explicit Exc(const std::string& aMsg):
            mMsg(QString::fromStdString(aMsg))
            {}

        explicit Exc(const QString& aMsg):
            mMsg(aMsg)
            {}

        virtual ~Exc() throw (){}
        virtual const char* what() const throw (){
            return qPrintable(mMsg);
        }

    protected:
        QString mMsg;
    };

} //namespace media

#endif //MEDIAMODEL_EXCEPTIONS_H
