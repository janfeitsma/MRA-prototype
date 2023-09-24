#include "backend.hpp"
#include "control.hpp"
#include "json_convert.hpp"
#include "spdlogformatter.hpp" // our customizations
#include "logdebug.hpp"
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
    if (cfg.enabled() && cfg.dumpticks())
    {
        std::string binfolder = MRA::Logging::control::getLogFolder() + "/tickbins";
        if (!std::filesystem::exists(binfolder)) {
            std::filesystem::create_directory(binfolder);
        }
        std::string filename = binfolder + "/" + "tick_" + componentName + "_" + std::to_string(counter) + ".bin";
        if (std::filesystem::exists(filename)) {
            // TODO: with nested components, this can happen, may need some context/folder?
            return NULL; // hack it for now
            throw std::runtime_error("file already exists: " + filename);
        }
        LOGDEBUG("dumping binary data to %s", filename.c_str());
        return new std::ofstream(filename);
    }
    return NULL;
}

// dump protobuf data to binary file
template <typename T>
void dumpToFile(T const &pbObject, std::ofstream *fp)
{
    if (!fp || !fp->is_open()) return;
    // serialize the protobuf object to a string
    std::ostringstream oss;
    pbObject.SerializeToOstream(&oss);
    std::string serializedData = oss.str();
    // write the byte count followed by the serialized object
    int byteCount = static_cast<int>(serializedData.size());
    fp->write(reinterpret_cast<const char*>(&byteCount), sizeof(int));
    fp->write(serializedData.c_str(), byteCount);
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
        // prepare strings for logging with INFO and TRACE levels
        // state and local may grow large -> these go to tracing, not info
        std::string infoStr = "\"tick\":" + std::to_string(counter)
            + ",\"timestamp\":" + google::protobuf::util::TimeUtil::ToString(timestamp)
            + ",\"input\":" + inputStr
            + ",\"params\":" + paramsStr;
        std::string traceStr = infoStr + ",\"state_in\":" + stateStr;
        logger->log(loc, MRA::Logging::TRACE, "> {%s}", traceStr.c_str());
        logger->log(loc, MRA::Logging::INFO, "start {%s}", infoStr.c_str());
        // tick .bin dump
        if (cfg.dumpticks() && (binfile != NULL))
        {
            dumpToFile(input, binfile);
            dumpToFile(params, binfile);
            dumpToFile(state, binfile);
        }
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
        // prepare strings for logging with INFO and TRACE levels
        // state and local may grow large -> these go to tracing, not info
        std::string infoStr = "\"tick\":" + std::to_string(counter)
            + ",\"error_value\":" + std::to_string(error_value)
            + ",\"duration\":" + std::to_string(duration)
            + ",\"output\":" + outputStr;
        std::string traceStr = infoStr + ",\"state_out\":" + stateStr;
        logger->log(loc, MRA::Logging::INFO, "end {%s}", infoStr.c_str());
        logger->log(loc, MRA::Logging::TRACE, "< {%s}", traceStr.c_str());
        // tick .bin dump
        if (cfg.dumpticks() && (binfile != NULL))
        {
            dumpToFile(output, binfile);
            dumpToFile(diag, binfile);
            dumpToFile(state, binfile);
            binfile->close();
            delete binfile;
        }
    }
}

// configuration management
void reconfigure(MRA::Datatypes::LogSpec const &cfg)
{
    // keep current configuration in memory
    static MRA::Datatypes::LogSpec currentCfg;
    // only reconfigure upon change
    // protobuf c++ API does not provide (in-)equality operators - use json conversion (or create a Configuration class?)
    if (MRA::convert_proto_to_json_str(currentCfg) != MRA::convert_proto_to_json_str(cfg) || s_logger == NULL)
    {
        LOGDEBUG("reconfigure %s -> %s", MRA::convert_proto_to_json_str(currentCfg).c_str(), MRA::convert_proto_to_json_str(cfg).c_str());
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
    case MRA::Logging::TRACE:    return spdlog::level::level_enum::trace;
    default: ;
    }
    throw std::runtime_error(std::string("Unrecognized log_level, int value " + std::to_string(int(log_level))));
}

std::shared_ptr<MraLogger> MraLogger::getInstance()
{
    if (s_logger == NULL) {
        s_logger = std::shared_ptr<MraLogger>(new MraLogger());
    }
    return s_logger;
}

void clear()
{
    s_logger.reset();
}

MraLogger::MraLogger()
{
    LOGDEBUG("construct MraLogger");
    m_active = false;
    m_filename_pattern = MRA::Logging::control::getFileNamePattern();
    // for the remainder: lazy setup: at first logger call
}

MraLogger::~MraLogger()
{
    m_spdlog_logger.reset();
    spdlog::drop(m_log_name);
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
    LOGDEBUG("setup m_active=%d", m_active);
    if (!m_active) return;

    auto log_level_mra = (MRA::Logging::LogLevel)(int)cfg.level();
    auto log_level_spd = convert_log_level(log_level_mra);
    spdlog::set_formatter(make_formatter(cfg.pattern()));
    spdlog::set_level(log_level_spd);

    // Logger construction only happens once per process.
    // No runtime reconfiguration for this part.
    if (m_spdlog_logger == NULL) {
        // Determine log name and file.
        m_log_name = "MRA:" + cfg.component();
        m_log_file = MRA::Logging::control::getLogFolder() + "/" + determineFileName(cfg.component());

        // Create the logger
        LOGDEBUG("spdlog create %s %s", m_log_name.c_str(), m_log_file.c_str());
        m_spdlog_logger = spdlog::basic_logger_mt(m_log_name, m_log_file);
        // TODO: consider using <spdlog::async_factory> for performance?
        // but then check that __FILE__ logging does not become garbage
        // see also https://github.com/gabime/spdlog/issues/2867
    }

    // Configure logger
    m_spdlog_logger->set_level(log_level_spd);
    if (cfg.hotflush()) {
        m_spdlog_logger->flush_on(log_level_spd);
    }
}

void MraLogger::setPreLogText(const std::string& r_pretext)
{
    m_pretext = r_pretext;
}

std::string sanitize(std::string const &s)
{
    std::string result = s;
    std::string search = "\n", replace = "\\n";
    size_t pos;
    while ((pos = result.find(search)) != std::string::npos) {
        result.replace(pos, 1, replace);
    }
    return result;
}

void MraLogger::log(source_loc loc, MRA::Logging::LogLevel loglevel, const char *fmt,...)
{
    spdlog::source_loc loc_spd{loc.filename, loc.line, loc.funcname};
    if (m_active) {
        LOGDEBUG("log[%s] %s(%d):%s()", spdlog::level::to_string_view(convert_log_level(loglevel)).data(), loc.filename, loc.line, loc.funcname);
        MRA::Logging::setComponentName(loc.componentname); // for %k custom formatter
        const int MAXTEXT = 4096; // TODO use configuration
        char buffer[MAXTEXT];
        buffer[MAXTEXT-1] = '\0';
        va_list argptr;
        va_start(argptr, fmt);
        int count = vsnprintf(buffer, MAXTEXT, fmt, argptr);
        va_end(argptr);
        // prevent overflow
        if (count > MAXTEXT)
        {
            // Truncate the string and add "..."
            buffer[MAXTEXT - 4] = '.';
            buffer[MAXTEXT - 3] = '.';
            buffer[MAXTEXT - 2] = '.';
            buffer[MAXTEXT - 1] = '\0';
        }
        // sanitize string
        std::string s = sanitize(m_pretext + buffer);
        switch (loglevel) {
        case MRA::Logging::CRITICAL:
            m_spdlog_logger->log(loc_spd, spdlog::level::critical, s);
            break;
        case MRA::Logging::ERROR:
            m_spdlog_logger->log(loc_spd, spdlog::level::err, s);
            break;
        case MRA::Logging::WARNING:
            m_spdlog_logger->log(loc_spd, spdlog::level::warn, s);
            break;
        case MRA::Logging::INFO:
            m_spdlog_logger->log(loc_spd, spdlog::level::info, s);
            break;
        case MRA::Logging::DEBUG:
            m_spdlog_logger->log(loc_spd, spdlog::level::debug, s);
            break;
        case MRA::Logging::TRACE:
            m_spdlog_logger->log(loc_spd, spdlog::level::trace, s);
            break;
        }
        va_end(argptr);
        // TODO: why is flush needed here, why doesn't flush_on at setup() seem to work?
        m_spdlog_logger->flush();
    }
    else {
        LOGDEBUG("log INACTIVE");
    }
}


MraLogger::FunctionRecord::FunctionRecord(source_loc loc)
    : _loc(loc)
{
    LOGDEBUG("FunctionRecord");
    auto cfg = control::getConfiguration(loc.componentname);
    if (cfg.enabled())
    {
        reconfigure(cfg);
    }
}

MraLogger::FunctionRecord::~FunctionRecord()
{
    flush_output();
}

void MraLogger::FunctionRecord::add_input(std::string const &varname, int value)
{
    _input_data.emplace_back(varname, value);
}

void MraLogger::FunctionRecord::add_input(std::string const &varname, float value)
{
    _input_data.emplace_back(varname, value);
}

void MraLogger::FunctionRecord::add_input(std::string const &varname, double value)
{
    _input_data.emplace_back(varname, value);
}

void MraLogger::FunctionRecord::add_input(std::string const &varname, bool value)
{
    _input_data.emplace_back(varname, value);
}

void MraLogger::FunctionRecord::add_input(std::string const &varname, std::string const &value)
{
    _input_data.emplace_back(varname, value);
}

void MraLogger::FunctionRecord::add_input(std::string const &varname, google::protobuf::Message const &value)
{
    _input_data.emplace_back(varname, convert_proto_to_json_str(value));
}

void MraLogger::FunctionRecord::add_output(std::string const &varname, int value)
{
    _output_data.emplace_back(varname, value);
}

void MraLogger::FunctionRecord::add_output(std::string const &varname, float value)
{
    _output_data.emplace_back(varname, value);
}

void MraLogger::FunctionRecord::add_output(std::string const &varname, double value)
{
    _output_data.emplace_back(varname, value);
}

void MraLogger::FunctionRecord::add_output(std::string const &varname, bool value)
{
    _output_data.emplace_back(varname, value);
}

void MraLogger::FunctionRecord::add_output(std::string const &varname, std::string const &value)
{
    _output_data.emplace_back(varname, value);
}

void MraLogger::FunctionRecord::add_output(std::string const &varname, google::protobuf::Message const &value)
{
    _output_data.emplace_back(varname, convert_proto_to_json_str(value));
}

std::string MraLogger::FunctionRecord::_convert_to_json(std::vector<std::pair<std::string, std::variant<int, double, bool, std::string>>> const &data)
{
    std::string js = "{";
    bool first = true;
    for (const auto& item : data) {
        if (!first) js += ",";
        first = false;
        js += "\"" + item.first + "\":";
        if (std::holds_alternative<int>(item.second)) {
            js += std::to_string(std::get<int>(item.second));
        } else if (std::holds_alternative<double>(item.second)) {
            js += std::to_string(std::get<double>(item.second));
        } else if (std::holds_alternative<bool>(item.second)) {
            js += (std::get<bool>(item.second) ? "true" : "false");
        } else if (std::holds_alternative<std::string>(item.second)) {
            js += "\"" + std::get<std::string>(item.second) + "\"";
        }
    }
    js += "}";
    return js;
}

void MraLogger::FunctionRecord::flush_input()
{
    std::string js = _convert_to_json(_input_data);
    MRA::Logging::backend::MraLogger::getInstance()->log(_loc, MRA::Logging::TRACE, "> %s", js.c_str());
}

void MraLogger::FunctionRecord::flush_output()
{
    std::string js = _convert_to_json(_output_data);
    MRA::Logging::backend::MraLogger::getInstance()->log(_loc, MRA::Logging::TRACE, "< %s", js.c_str());
}

} // namespace MRA::Logging::backend

