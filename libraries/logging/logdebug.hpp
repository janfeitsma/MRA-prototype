#ifndef _MRA_LIBRARIES_LOGGING_LOGDEBUG_HPP
#define _MRA_LIBRARIES_LOGGING_LOGDEBUG_HPP


// uncomment to enable internal debug logging
// this is intended only for developing libraries/logging

//#define LOGDEBUG_ENABLED


#ifdef LOGDEBUG_ENABLED
#include <cstdio>

// Define the LOGDEBUG macro
#define LOGDEBUG(fmt, ...) \
    do { \
        if constexpr (true) { \
            std::printf("%s(%d) - ", __FILE__, __LINE__); \
            std::printf(fmt, ##__VA_ARGS__); \
            std::printf("\n"); \
            std::fflush(stdout); \
        } \
    } while (false)

#else // LOGDEBUG_ENABLED is not defined, define LOGDEBUG as empty
#define LOGDEBUG(fmt, ...) do {} while (false)
#endif // LOGDEBUG_ENABLED


#endif // #ifndef _MRA_LIBRARIES_LOGGING_LOGDEBUG_HPP

