#ifndef _MRA_LIBRARIES_LOGGING_MACROS_HPP
#define _MRA_LIBRARIES_LOGGING_MACROS_HPP

#include "logtick.hpp"
#include "backend.hpp"


#define MRA_LOG_TICK() MRA::Logging::LogTick scoped(MRA_COMPONENT_NAME, __FILE__, __LINE__, timestamp, input, params, &state, &output, &local, &error_value)

// MRA_COMPONENT_NAME may be undefined, when using macros in internal implementation -> provide fallback
#ifndef MRA_COMPONENT_NAME
#define MRA_COMPONENT_NAME "unknown"
#endif

#define MRA_LOG_CRITICAL(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, MRA_COMPONENT_NAME, __LINE__, __FUNCTION__) , MRA::Logging::CRITICAL, __VA_ARGS__ )

#define MRA_LOG_ERROR(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, MRA_COMPONENT_NAME, __LINE__, __FUNCTION__) , MRA::Logging::ERROR, __VA_ARGS__ )

#define MRA_LOG_WARNING(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, MRA_COMPONENT_NAME, __LINE__, __FUNCTION__) , MRA::Logging::WARNING, __VA_ARGS__ )

#define MRA_LOG_INFO(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, MRA_COMPONENT_NAME, __LINE__, __FUNCTION__) , MRA::Logging::INFO, __VA_ARGS__ )

#define MRA_LOG_DEBUG(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, MRA_COMPONENT_NAME, __LINE__, __FUNCTION__) , MRA::Logging::DEBUG, __VA_ARGS__ )

// trace function call and duration

#define MRA_TRACE_FUNCTION() MRA::Logging::backend::MraLogger::FunctionRecord scoped(\
    MRA::Logging::backend::source_loc(__FILE__, MRA_COMPONENT_NAME, __LINE__, __FUNCTION__) ); \
    scoped.flush_input()

// gtest specialization to show actual test case name instead of generic "TestBody"

#define MRA_TRACE_TEST_FUNCTION() MRA::Logging::backend::MraLogger::FunctionRecord scoped(\
    MRA::Logging::backend::source_loc(__FILE__, MRA_COMPONENT_NAME, __LINE__, test_info_->name()) ); \
    scoped.flush_input()

// single-argument function i/o logging

#define MRA_TRACE_FUNCTION_INPUT(varname) \
    MRA::Logging::backend::MraLogger::FunctionRecord scoped( \
        MRA::Logging::backend::source_loc(__FILE__, MRA_COMPONENT_NAME, __LINE__, __FUNCTION__) \
    ); \
    scoped.add_input(#varname, varname); \
    scoped.flush_input()
#define MRA_TRACE_FUNCTION_OUTPUT(varname) scoped.add_output(#varname, varname)

// multi-argument function i/o logging

#include "macromap.h"

#define MRA_TRACE_FUNCTION_INPUT_PAIR(v) scoped.add_input(#v, v);
#define MRA_TRACE_FUNCTION_INPUTS(...) \
    MRA::Logging::backend::MraLogger::FunctionRecord scoped( \
        MRA::Logging::backend::source_loc(__FILE__, MRA_COMPONENT_NAME, __LINE__, __FUNCTION__) \
    ); \
    MAP(MRA_TRACE_FUNCTION_INPUT_PAIR, __VA_ARGS__) \
    scoped.flush_input()

#endif // #ifndef _MRA_LIBRARIES_LOGGING_MACROS_HPP

