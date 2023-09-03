#ifndef _MRA_LIBRARIES_MRA_LOGGER_BACKEND_HPP
#define _MRA_LIBRARIES_MRA_LOGGER_BACKEND_HPP

#include "datatypes/Logging.pb.h"
#include "levels.hpp"
#include <memory>
#include <string>
#include <google/protobuf/util/time_util.h>
#include "spdlog/spdlog.h"  // spdlog API: https://github.com/gabime/spdlog


namespace MRA::Logging::backend
{

// tick logging: get binary file if configured, or NULL pointer
std::ofstream *logTickBinFile(
    MRA::Datatypes::LogSpec const &cfg,
    std::string const &componentName,
    int counter);

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
    google::protobuf::Message const &state);

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
    google::protobuf::Message const &diag);

// configuration management
spdlog::level::level_enum convert_log_level(MRA::Logging::LogLevel log_level);
void reconfigure(MRA::Datatypes::LogSpec const &cfg);


class MraLogger
{
public:

    static std::shared_ptr<MraLogger> getInstance();

    void log(MRA::Logging::LogLevel loglevel, const char *fmt,...);

    void setPreLogText(const std::string& r_pretext);

    MraLogger(const MraLogger& obj) = delete;

    void setup(bool active, std::string const& log_pattern, MRA::Logging::LogLevel log_level, std::string const &log_name, std::string const &log_file);

private:
    MraLogger();

    bool m_active = false;
    std::string m_pretext = "";
    std::shared_ptr<spdlog::logger> m_spdlog_logger;

}; // class MraLogger


} // namespace MRA::Logging::backend

#endif // #ifndef _MRA_LIBRARIES_MRA_LOGGER_BACKEND_HPP

