#ifndef _MRA_LIBRARIES_LOGGING_MACROS_HPP
#define _MRA_LIBRARIES_LOGGING_MACROS_HPP

#include "logtick.hpp"
#include "backend.hpp"


#define MRA_LOG_TICK() MRA::Logging::LogTick scoped(componentName(), __FILE__, __LINE__, timestamp, input, params, &state, &output, &local, &error_value)

// componentName may be undefined, when using macros in internal implementation -> provide fallback 'weak' function
__attribute__((weak)) std::string componentName() {
    return "unknown";
}

#define MRA_LOG_CRITICAL(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) , MRA::Logging::CRITICAL, __VA_ARGS__ )

#define MRA_LOG_ERROR(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) , MRA::Logging::ERROR, __VA_ARGS__ )

#define MRA_LOG_WARNING(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) , MRA::Logging::WARNING, __VA_ARGS__ )

#define MRA_LOG_INFO(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) , MRA::Logging::INFO, __VA_ARGS__ )

#define MRA_LOG_DEBUG(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) , MRA::Logging::DEBUG, __VA_ARGS__ )

#define MRA_TRACE_FUNCTION() MRA::Logging::backend::MraLogger::log_function scoped(\
    MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) )

// single-argument function i/o logging

#define MRA_TRACE_FUNCTION_INPUT(varname) \
    MRA::Logging::backend::MraLogger::FunctionRecord scoped( \
        MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) \
    ); \
    scoped.add_input(#varname, varname); \
    scoped.flush_input()
#define MRA_TRACE_FUNCTION_OUTPUT(varname) scoped.add_output(#varname, varname)

// preprocessor magic to support N i/o arguments
// https://stackoverflow.com/a/26408195
// maybe this could be done nicer? (like msgpack, using templates, ...)

#define __NARG__(...)  __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define __RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0

#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

#define MRA_TRACE_FUNCTION_INPUTS(...) VFUNC(MRA_TRACE_FUNCTION_INPUTS, __VA_ARGS__)

#define MRA_TRACE_FUNCTION_INPUTS0() \
    MRA::Logging::backend::MraLogger::FunctionRecord scoped( \
        MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) \
    ); \
    scoped.flush_input()

#define MRA_TRACE_FUNCTION_INPUTS1(v1) \
    MRA::Logging::backend::MraLogger::FunctionRecord scoped( \
        MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) \
    ); \
    scoped.add_input(#v1, v1); \
    scoped.flush_input()

#define MRA_TRACE_FUNCTION_INPUTS2(v1, v2) \
    MRA::Logging::backend::MraLogger::FunctionRecord scoped( \
        MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) \
    ); \
    scoped.add_input(#v1, v1); \
    scoped.add_input(#v2, v2); \
    scoped.flush_input()

#define MRA_TRACE_FUNCTION_INPUTS3(v1, v2, v3) \
    MRA::Logging::backend::MraLogger::FunctionRecord scoped( \
        MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) \
    ); \
    scoped.add_input(#v1, v1); \
    scoped.add_input(#v2, v2); \
    scoped.add_input(#v3, v3); \
    scoped.flush_input()

#endif // #ifndef _MRA_LIBRARIES_LOGGING_MACROS_HPP

