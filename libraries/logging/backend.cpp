#include "backend.hpp"
#include "json_convert.hpp"
#include <memory>
#include "spdlog/spdlog.h"  // spdlog API: https://github.com/gabime/spdlog
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <stdarg.h>



// tick logging: get binary file if configured, or NULL pointer
std::ofstream *MRA::Logging::backend::logTickBinFile(
    MRA::Datatypes::LogSpec const &cfg,
    std::string const &componentName,
    int counter)
{
    return NULL;
}

// tick logging: write logging/data at start of tick
void MRA::Logging::backend::logTickStart(
    std::string const &componentName,
    std::string const &fileName,
    int lineNumber,
    MRA::Datatypes::LogSpec const &cfg,
    std::ofstream *binfile,
    int counter,
    google::protobuf::Timestamp const &timestamp,
    google::protobuf::Message const &input,
    google::protobuf::Message const &params,
    google::protobuf::Message const &state)
{
    if (cfg.enabled())
    {
        auto logger = MraLogger::getInstance();
        // convert protobuf objects to string
        std::string inputStr = convert_proto_to_json_str(input);
        std::string paramsStr = convert_proto_to_json_str(params);
        std::string stateStr = convert_proto_to_json_str(state);
        // TODO pass sourceloc, make it the first arg of log, redirect to inner spdlog handlers
        logger->log(MRA::Logging::INFO, "tick %d START", counter);
        logger->log(MRA::Logging::INFO, "timestamp: %s", google::protobuf::util::TimeUtil::ToString(timestamp).c_str());
        logger->log(MRA::Logging::INFO, "input: %s", inputStr.c_str());
        logger->log(MRA::Logging::INFO, "params: %s", paramsStr.c_str());
        logger->log(MRA::Logging::INFO, "state: %s", stateStr.c_str());
        // TODO tick bindump, if configured
    }
}

// tick logging: write logging/data at end of tick
void MRA::Logging::backend::logTickEnd(
    std::string const &componentName,
    std::string const &fileName,
    int lineNumber,
    MRA::Datatypes::LogSpec const &cfg,
    std::ofstream *binfile,
    int counter,
    double duration,
    int error_value,
    google::protobuf::Message const &state,
    google::protobuf::Message const &output,
    google::protobuf::Message const &diag)
{
    if (cfg.enabled())
    {
        auto logger = MraLogger::getInstance();
        // convert protobuf objects to string
        std::string stateStr = convert_proto_to_json_str(state);
        std::string outputStr = convert_proto_to_json_str(output);
        logger->log(MRA::Logging::INFO, "tick %d END error_value=%d", counter, error_value);
        logger->log(MRA::Logging::INFO, "duration: %9.6f", duration);
        logger->log(MRA::Logging::INFO, "output: %s", outputStr.c_str());
        logger->log(MRA::Logging::INFO, "state: %s", stateStr.c_str());
        // TODO tick bindump, if configured
    }
}


using namespace MRA::Logging::backend;

std::shared_ptr<MraLogger> MraLogger::getInstance() {
    static std::shared_ptr<MraLogger> sp_logger(new MraLogger);
    return sp_logger;
}

MraLogger::MraLogger() {
    setup();
}

void MraLogger::setup()
{
    m_active = true;

    // microsecond resolution: %f
    // note that timestamps are absolute, in UTC
    // so for postprocessing & readability, timezone offset needs to be applied
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%f] [%n] [%^%l%$] %v");
    spdlog::set_level(spdlog::level::info);

    m_spdlog_logger = spdlog::basic_logger_mt<spdlog::async_factory>("MRA", "/tmp/async_log.txt");
}

void MraLogger::setPreLogText(const std::string& r_pretext) {
    m_pretext = r_pretext;
}

void MraLogger::log(MRA::Logging::LogLevel loglevel, const char *fmt,...)
{
    if (m_active) {
        const int MAXTEXT = 4096;
        char buffer[MAXTEXT];
        buffer[MAXTEXT-1] = '\0';
        va_list argptr;
        va_start(argptr, fmt);
        vsprintf(buffer, fmt, argptr);
        va_end(argptr);
        if (buffer[MAXTEXT-1])
        {
            buffer[MAXTEXT-1] = '\0';
            m_spdlog_logger->critical("fatal error, string overflow in %s\n", __FUNCTION__);
            m_spdlog_logger->critical("tried to process:\n%s...\n", buffer);
            exit(-1);
        }
        else {
            switch (loglevel) {
            case MRA::Logging::CRITICAL:
                m_spdlog_logger->critical(m_pretext + buffer);
                break;
            case MRA::Logging::ERROR:
                m_spdlog_logger->error(m_pretext + buffer);
                break;
            case MRA::Logging::WARNING:
                m_spdlog_logger->warn(m_pretext + buffer);
                break;
            case MRA::Logging::INFO:
                m_spdlog_logger->info(m_pretext + buffer);
                break;
            case MRA::Logging::DEBUG:
                m_spdlog_logger->debug(m_pretext + buffer);
                break;
            case MRA::Logging::TRACE:
                m_spdlog_logger->trace(m_pretext + buffer);
                break;
            }
        }
        va_end(argptr);
    }
}

