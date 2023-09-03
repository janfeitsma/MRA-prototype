#ifndef _MRA_LIBRARIES_LOGGING_MACROS_HPP
#define _MRA_LIBRARIES_LOGGING_MACROS_HPP


#define MRA_LOG_TICK() MRA::Logging::LogTick scoped(componentName(), __FILE__, __LINE__, timestamp, input, params, &state, &output, &local, &error_value)


#endif // #ifndef _MRA_LIBRARIES_LOGGING_MACROS_HPP

