#include "backend.hpp"
#include "control.hpp"
#include "json_convert.hpp"
#include <memory>
#include "spdlog/spdlog.h"  // spdlog API: https://github.com/gabime/spdlog
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <stdarg.h>

namespace MRA::Logging::backend
{

// tick logging: get binary file if configured, or NULL pointer
std::ofstream *logTickBinFile(
    MRA::Datatypes::LogSpec const &cfg,
    std::string const &componentName,
    int counter)
{
    return NULL;
}

// tick logging: write logging/data at start of tick
void logTickStart(
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
        std::string inputStr = MRA::convert_proto_to_json_str(input);
        std::string paramsStr = MRA::convert_proto_to_json_str(params);
        std::string stateStr = MRA::convert_proto_to_json_str(state);
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
void logTickEnd(
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
        std::string stateStr = MRA::convert_proto_to_json_str(state);
        std::string outputStr = MRA::convert_proto_to_json_str(output);
        logger->log(MRA::Logging::INFO, "tick %d END error_value=%d", counter, error_value);
        logger->log(MRA::Logging::INFO, "duration: %9.6f", duration);
        logger->log(MRA::Logging::INFO, "output: %s", outputStr.c_str());
        logger->log(MRA::Logging::INFO, "state: %s", stateStr.c_str());
        // TODO tick bindump, if configured
    }
}

// configuration management
void reconfigure(MRA::Datatypes::LogSpec const &cfg)
{
    // TODO: this might not yet support multiple components in the same process
    // keep current configuration in memory
    static MRA::Datatypes::LogSpec currentCfg;
    // only reconfigure upon change
    // protobuf c++ API does not provide (in-)equality operators - use json conversion (or create a Configuration class?)
    if (MRA::convert_proto_to_json_str(currentCfg) != MRA::convert_proto_to_json_str(cfg))
    {
        if (cfg.component().size() == 0) {
            throw std::runtime_error("Missing component name");
        }
        std::string name = "MRA:" + cfg.component();
        std::string log_file = MRA::Logging::control::logFolder() + "/" + cfg.component();
        MraLogger::getInstance()->setup(cfg.enabled(), cfg.pattern(), (MRA::Logging::LogLevel)(int)cfg.level(), name, log_file);
        currentCfg = cfg;
    }
}

// log level mapping
spdlog::level::level_enum convert_log_level(MRA::Logging::LogLevel log_level)
{
    switch (log_level)
    {
    case MRA::Logging::CRITICAL: return spdlog::level::level_enum::critical;
    case MRA::Logging::ERROR:    return spdlog::level::level_enum::err;
    case MRA::Logging::WARNING:  return spdlog::level::level_enum::warn;
    case MRA::Logging::INFO:     return spdlog::level::level_enum::info;
    case MRA::Logging::DEBUG:    return spdlog::level::level_enum::debug;
    default: ;
    }
    throw std::runtime_error(std::string("Unrecognized log_level, int value " + std::to_string(int(log_level))));
}

std::shared_ptr<MraLogger> MraLogger::getInstance()
{
    static std::shared_ptr<MraLogger> sp_logger(new MraLogger);
    return sp_logger;
}

MraLogger::MraLogger()
{
    // lazy setup: at first logger call
}

void MraLogger::setup(bool active, std::string const& log_pattern, MRA::Logging::LogLevel log_level, std::string const &log_name, std::string const &log_file)
{
    m_active = active;
    spdlog::set_pattern(log_pattern.c_str());
    spdlog::set_level(convert_log_level(log_level));

    // Check if the logger already exists
    auto existing_logger = spdlog::get(log_name);
    if (existing_logger) {
        // Logger with the same name already exists, update its properties
        existing_logger->set_pattern(log_pattern);
        existing_logger->set_level(convert_log_level(log_level));
        //existing_logger->set_filename(log_file); // TODO: how to runtime reconfigure?
    } else {
        // Logger with the given name doesn't exist, create a new one
        m_spdlog_logger = spdlog::basic_logger_mt<spdlog::async_factory>(log_name, log_file);
    }
}

void MraLogger::setPreLogText(const std::string& r_pretext)
{
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

} // namespace MRA::Logging::backend

