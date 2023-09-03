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
        MRA::Logging::backend::source_loc loc{fileName.c_str(), lineNumber, "tick"};
        logger->log(loc, MRA::Logging::INFO, "tick %d START", counter);
        logger->log(loc, MRA::Logging::INFO, "timestamp: %s", google::protobuf::util::TimeUtil::ToString(timestamp).c_str());
        logger->log(loc, MRA::Logging::INFO, "input: %s", inputStr.c_str());
        logger->log(loc, MRA::Logging::INFO, "params: %s", paramsStr.c_str());
        logger->log(loc, MRA::Logging::INFO, "state(in): %s", stateStr.c_str());
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
        MRA::Logging::backend::source_loc loc{fileName.c_str(), lineNumber, "tick"};
        logger->log(loc, MRA::Logging::INFO, "tick %d END error_value=%d", counter, error_value);
        logger->log(loc, MRA::Logging::INFO, "duration: %9.6f", duration);
        logger->log(loc, MRA::Logging::INFO, "output: %s", outputStr.c_str());
        logger->log(loc, MRA::Logging::INFO, "state: %s", stateStr.c_str());
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
        MraLogger::getInstance()->setup(cfg);
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

void MraLogger::setup(MRA::Datatypes::LogSpec const &cfg)
{
    m_active = cfg.enabled();
    auto log_level_mra = (MRA::Logging::LogLevel)(int)cfg.level();
    auto log_level_spd = convert_log_level(log_level_mra);
    spdlog::set_pattern(cfg.pattern().c_str());
    spdlog::set_level(log_level_spd);
    std::string log_name = "MRA:" + cfg.component();
    std::string log_file = MRA::Logging::control::getLogFolder() + "/" + cfg.component() + ".log";

    // Check if the logger already exists
    if (!spdlog::get(log_name)) {
        // Logger with the given name doesn't exist, create a new one
        m_spdlog_logger = spdlog::basic_logger_mt<spdlog::async_factory>(log_name, log_file);
    }

    // Configure logger
    m_spdlog_logger->set_pattern(cfg.pattern());
    m_spdlog_logger->set_level(log_level_spd);
    //m_spdlog_logger->set_filename(log_file); // TODO: how to runtime reconfigure? access and manipulate sink(s)
    if (cfg.hotflush()) {
        m_spdlog_logger->flush_on(log_level_spd);
    }
}

void MraLogger::setPreLogText(const std::string& r_pretext)
{
    m_pretext = r_pretext;
}

void MraLogger::log(source_loc const &loc, MRA::Logging::LogLevel loglevel, const char *fmt,...)
{
    if (m_active) {
        const int MAXTEXT = 4096; // TODO use configuration
        char buffer[MAXTEXT];
        buffer[MAXTEXT-1] = '\0';
        va_list argptr;
        va_start(argptr, fmt);
        vsprintf(buffer, fmt, argptr);
        va_end(argptr);
        spdlog::source_loc loc_spd{loc.filename, loc.line, loc.funcname};
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
                m_spdlog_logger->log(loc_spd, spdlog::level::critical, m_pretext + buffer);
                break;
            case MRA::Logging::ERROR:
                m_spdlog_logger->log(loc_spd, spdlog::level::err, m_pretext + buffer);
                break;
            case MRA::Logging::WARNING:
                m_spdlog_logger->log(loc_spd, spdlog::level::warn, m_pretext + buffer);
                break;
            case MRA::Logging::INFO:
                m_spdlog_logger->log(loc_spd, spdlog::level::info, m_pretext + buffer);
                break;
            case MRA::Logging::DEBUG:
                m_spdlog_logger->log(loc_spd, spdlog::level::debug, m_pretext + buffer);
                break;
            case MRA::Logging::TRACE:
                m_spdlog_logger->log(loc_spd, spdlog::level::trace, m_pretext + buffer);
                break;
            }
        }
        va_end(argptr);
        // TODO: why is flush needed here, why doesn't flush_on at setup() seem to work?
        m_spdlog_logger->flush();
    }
}

} // namespace MRA::Logging::backend

