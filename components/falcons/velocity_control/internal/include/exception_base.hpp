#ifndef _EXCEPTIONS_BASE_HPP_
#define _EXCEPTIONS_BASE_HPP_


#include <exception>

// usage:
// 1. instantiate one or more custom exceptions :
//       DECLARE_CUSTOM_EXCEPTION(MyCustomException);
//       DECLARE_CUSTOM_EXCEPTION_DESCR(MyCustomException2, "custom description instead of MyCustomException2");
// 2. use them as follows:
//       throw MyCustomException(__FILE__, __LINE__);
//       throw MyCustomException(__FILE__, __LINE__, "some details");


// TODO: if this is considered useful and generic enough, then move to MRA::libraries?
// TODO: it would be nice if __FILE__ and __LINE__ were filled in automatically via some kind of macro expansion



// Custom exception template with file and line information
template <typename ExceptionType>
class CustomExceptionBase : public std::exception
{
public:
    CustomExceptionBase(const char* file, int line, std::string descr, std::string message="")
        : file_(file), line_(line), descr_(descr), message_(message) {}

    virtual const char* what() const noexcept override
    {
        static std::string detail_message_ = descr_;
        if (message_.size()) detail_message_ += ": " + message_;
        detail_message_ += " at line " + std::to_string(line_);
        detail_message_ += " of " + std::string(file_);
        return detail_message_.c_str();
    }

    const char* file() const noexcept
    {
        return file_;
    }

    int line() const noexcept
    {
        return line_;
    }

private:
    std::string descr_;
    std::string message_;
    const char* file_;
    int line_;
};


// Macro to define a custom exception class
#define DECLARE_CUSTOM_EXCEPTION(ExceptionName) \
    class ExceptionName : public CustomExceptionBase<ExceptionName> { \
    public: \
        ExceptionName(const char* file, int line, std::string message="") \
            : CustomExceptionBase<ExceptionName>(file, line, #ExceptionName, message) {} \
    }

#define DECLARE_CUSTOM_EXCEPTION_DESCR(ExceptionName, CustomDescription) \
    class ExceptionName : public CustomExceptionBase<ExceptionName> { \
    public: \
        ExceptionName(const char* file, int line, std::string message="") \
            : CustomExceptionBase<ExceptionName>(file, line, CustomDescription, message) {} \
    }



#endif // #ifndef _EXCEPTIONS_BASE_HPP_

