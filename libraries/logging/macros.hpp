#ifndef _MRA_LIBRARIES_LOGGING_MACROS_HPP
#define _MRA_LIBRARIES_LOGGING_MACROS_HPP

#include "logtick.hpp"
#include "backend.hpp"

#define MRA_LOG_TICK() MRA::Logging::LogTick scoped(componentName(), __FILE__, __LINE__, timestamp, input, params, &state, &output, &local, &error_value)

#define MRA_LOG_CRITICAL(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, __LINE__, __FUNCTION__) , MRA::Logging::CRITICAL, __VA_ARGS__ )
#define MRA_LOG_ERROR(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, __LINE__, __FUNCTION__) , MRA::Logging::ERROR, __VA_ARGS__ )
#define MRA_LOG_WARNING(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, __LINE__, __FUNCTION__) , MRA::Logging::WARNING, __VA_ARGS__ )
#define MRA_LOG_INFO(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, __LINE__, __FUNCTION__) , MRA::Logging::INFO, __VA_ARGS__ )
#define MRA_LOG_DEBUG(...) MRA::Logging::backend::MraLogger::getInstance()->log(\
    MRA::Logging::backend::source_loc(__FILE__, __LINE__, __FUNCTION__) , MRA::Logging::DEBUG, __VA_ARGS__ )

#endif // #ifndef _MRA_LIBRARIES_LOGGING_MACROS_HPP

