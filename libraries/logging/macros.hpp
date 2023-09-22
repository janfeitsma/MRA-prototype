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

#define MRA_TRACE_FUNCTION() MRA::Logging::backend::MraLogger::FunctionRecord scoped(\
    MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) )

// single-argument function i/o logging

#define MRA_TRACE_FUNCTION_INPUT(varname) \
    MRA::Logging::backend::MraLogger::FunctionRecord scoped( \
        MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) \
    ); \
    scoped.add_input(#varname, varname); \
    scoped.flush_input()
#define MRA_TRACE_FUNCTION_OUTPUT(varname) scoped.add_output(#varname, varname)

#include "macromap.h"

#define MRA_TRACE_FUNCTION_INPUT_PAIR(v) scoped.add_input(#v, v);
#define MRA_TRACE_FUNCTION_INPUTS(...) \
    MRA::Logging::backend::MraLogger::FunctionRecord scoped( \
        MRA::Logging::backend::source_loc(__FILE__, componentName().c_str(), __LINE__, __FUNCTION__) \
    ); \
    MAP(MRA_TRACE_FUNCTION_INPUT_PAIR, __VA_ARGS__) \
    scoped.flush_input()

#endif // #ifndef _MRA_LIBRARIES_LOGGING_MACROS_HPP

