#include "backend.hpp"
#include "control.hpp"
#include "json_convert.hpp"
#include <memory>
#include "spdlog/spdlog.h"  // spdlog API: https://github.com/gabime/spdlog
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <stdarg.h>
#include <errno.h> // for program_invocation_name


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
        MRA::Logging::backend::source_loc loc{fileName.c_str(), componentName.c_str(), lineNumber, "tick"};
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
        MRA::Logging::backend::source_loc loc{fileName.c_str(), componentName.c_str(), lineNumber, "tick"};
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
    m_filename_pattern = MRA::Logging::control::getFileNamePattern();
    // for the remainder: lazy setup: at first logger call
}

void MraLogger::setFileName(std::string const &f)
{
    m_filename_pattern = f;
}

void replaceAll(std::string &s, const std::string &search, const std::string &replace) {
    size_t pos = 0;
    while ((pos = s.find(search, pos)) != std::string::npos) {
        s.replace(pos, search.length(), replace);
        pos += replace.length();
    }
}

std::string MraLogger::determineFileName(std::string const &cname)
{
    // Default file name configuration is something like "<maincomponent>_<pid>.log".
    // The <placeholders> need to be filled in.
    // Get the pattern. It may be customized at start of main execution using logger setFileName.
    std::string result = m_filename_pattern;
    // replace <maincomponent> - rely on being called first
    replaceAll(result, "<maincomponent>", cname);
    // replace <pid> number
    int pid = (int)getpid();
    replaceAll(result, "<pid>", std::to_string(pid));
    // replace <pidstr>
    replaceAll(result, "<pidstr>", program_invocation_short_name); // from <errno.h>, perhaps not very portable, but there are no windows users on current radar
    // do not use raw program_invocation_name as it is way too long,
    // example from bazel testsuite: /home/jan/.cache/bazel/_bazel_jan/de3e7735840c0362d277fa685ddbcd35/sandbox/linux-sandbox/29/execroot/MRA/bazel-out/k8-fastbuild/bin/components/falcons/localization_vision/testsuite.runfiles/MRA/components/falcons/localization_vision/testsuite
    // replace <date> and <time>
    std::time_t currentTime = std::time(nullptr);
    struct std::tm *timeinfo = std::localtime(&currentTime);
    char dateBuffer[9];
    std::strftime(dateBuffer, sizeof(dateBuffer), "%Y%m%d", timeinfo);
    char timeBuffer[7];
    std::strftime(timeBuffer, sizeof(timeBuffer), "%H%M%S", timeinfo);
    replaceAll(result, "<date>", dateBuffer);
    replaceAll(result, "<time>", timeBuffer);
    // Sanity check on result
    replaceAll(result, "<", "");
    replaceAll(result, ">", "");
    if (result.size() == 0)
    {
        throw std::runtime_error(std::string("Empty log filename"));
    }
    return result;
}

void MraLogger::setup(MRA::Datatypes::LogSpec const &cfg)
{
    m_active = cfg.enabled();
    if (!m_active) return;

    auto log_level_mra = (MRA::Logging::LogLevel)(int)cfg.level();
    auto log_level_spd = convert_log_level(log_level_mra);
    spdlog::set_pattern(cfg.pattern().c_str());
    spdlog::set_level(log_level_spd);

    // Logger construction only happens once per process.
    // No runtime reconfiguration for this part.
    if (m_spdlog_logger == NULL) {
        // Determine log name and file.
        std::string log_name = "MRA:" + cfg.component();
        std::string log_file = MRA::Logging::control::getLogFolder() + "/" + determineFileName(cfg.component());

        // Create the logger
        m_spdlog_logger = spdlog::basic_logger_mt(log_name, log_file);
        // TODO: consider using <spdlog::async_factory> for performance?
        // but then check that __FILE__ logging does not become garbage
        // see also https://github.com/gabime/spdlog/issues/2867
    }

    // Configure logger
    m_spdlog_logger->set_pattern(cfg.pattern());
    m_spdlog_logger->set_level(log_level_spd);
    if (cfg.hotflush()) {
        m_spdlog_logger->flush_on(log_level_spd);
    }
}

void MraLogger::setPreLogText(const std::string& r_pretext)
{
    m_pretext = r_pretext;
}

void MraLogger::log(source_loc loc, MRA::Logging::LogLevel loglevel, const char *fmt,...)
{
    spdlog::source_loc loc_spd{loc.filename, loc.line, loc.funcname};
    if (m_active) {
        const int MAXTEXT = 4096; // TODO use configuration
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

