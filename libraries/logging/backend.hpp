#ifndef _MRA_LIBRARIES_MRA_LOGGER_BACKEND_HPP
#define _MRA_LIBRARIES_MRA_LOGGER_BACKEND_HPP

#include "datatypes/Logging.pb.h"
#include "levels.hpp"
#include <memory>
#include <string>
#include <google/protobuf/util/time_util.h>
#define SPDLOG_ACTIVE_LEVEL TRACE
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


// inspired by spdlog
struct source_loc
{
    constexpr source_loc() = default;
    constexpr source_loc(const char *filename_in, const char *componentname_in, int line_in, const char *funcname_in)
        : filename{filename_in}
        , componentname{componentname_in}
        , line{line_in}
        , funcname{funcname_in}
    {}
    constexpr source_loc(const source_loc &other)
        : filename{other.filename}
        , componentname{other.componentname}
        , line{other.line}
        , funcname{other.funcname}
    {}

    constexpr bool empty() const noexcept
    {
        return line == 0;
    }

    const char *filename{nullptr};
    const char *componentname{nullptr};
    int line{0};
    const char *funcname{nullptr};
};

void clear();

class MraLogger
{
public:

    static std::shared_ptr<MraLogger> getInstance();

    void log(source_loc loc, MRA::Logging::LogLevel loglevel, const char *fmt,...);

    class log_function
    {
    public:
        log_function(source_loc loc);
        ~log_function();
    private:
        source_loc _loc;
    };

    void setPreLogText(const std::string& r_pretext);
    void setFileName(const std::string& filename);

    MraLogger(const MraLogger& obj) = delete;
    ~MraLogger();

    void setup(MRA::Datatypes::LogSpec const &cfg);


private:
    MraLogger();
    std::string determineFileName(std::string const &cname);

    bool m_active = false;
    std::string m_pretext = "";
    std::shared_ptr<spdlog::logger> m_spdlog_logger;
    std::string m_filename_pattern = "";
    std::string m_log_name;
    std::string m_log_file;

}; // class MraLogger

static std::shared_ptr<MraLogger> s_logger = NULL;

} // namespace MRA::Logging::backend

#endif // #ifndef _MRA_LIBRARIES_MRA_LOGGER_BACKEND_HPP

