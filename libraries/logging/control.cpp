#include "control.hpp"
#include "google/protobuf/util/json_util.h"
#include "json_convert.hpp"
#include "logdebug.hpp"

#include <iostream>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>

namespace MRA::Logging::control
{

namespace
{
    std::string ENVIRONMENT_KEY = "MRA_LOGGER_CONTEXT";
    std::string LOG_LEVEL_KEY = "MRA_LOG_LEVEL";
    std::string SHARED_MEMORY_FILE = "mra_logging_shared_memory";
    std::string DEFAULT_LOG_FOLDER = "mra_logging";
    const size_t SHARED_MEMORY_SIZE = 4096;
}

std::string _getenv()
{
    const char* env_value = getenv(ENVIRONMENT_KEY.c_str());
    if (env_value != nullptr) {
        return std::string(env_value);
    }
    return "";
}

std::string _mkShmFile()
{
    std::string envstr = _getenv();
    if (envstr.size())
    {
        std::string prefix = envstr;
        if (prefix.back() != '_') prefix += "_";
        return "/" + prefix + SHARED_MEMORY_FILE;
    }
    // no environment set
    return "/" + SHARED_MEMORY_FILE;
}

std::string _mkLogFolder()
{
    std::string envstr = _getenv();
    if (envstr.size())
    {
        std::string prefix = envstr;
        if (prefix.back() != '_') prefix += "_";
        return "/tmp/" + prefix + DEFAULT_LOG_FOLDER;
    }
    // no environment set
    return "/tmp/" + DEFAULT_LOG_FOLDER;
}

std::string getLogFolder()
{
    MRA::Datatypes::LogControl config = getConfiguration();
    std::string result = config.folder();
    if (std::filesystem::exists(result) && !std::filesystem::is_directory(result)) {
        throw std::runtime_error(std::string("Error determining log folder: '") + result + "' exists but is not a folder");
    }
    if (!std::filesystem::exists(result)) {
        std::filesystem::create_directory(result);
    }
    return result;
}

std::string getFileNamePattern()
{
    MRA::Datatypes::LogControl config = getConfiguration();
    return config.filename();
}

MRA::Datatypes::LogControl defaultConfiguration()
{
    MRA::Datatypes::LogControl result;
    result.set_folder(_mkLogFolder());
    result.set_filename("<maincomponent>_<pid>.log");
    result.mutable_general()->set_component("MRA");
    std::string level_str = "INFO";
    char const *cp = getenv(LOG_LEVEL_KEY.c_str());
    if (cp) level_str = cp;
    const google::protobuf::EnumDescriptor *descriptor = MRA::Datatypes::LogLevel_descriptor();
    result.mutable_general()->set_level((MRA::Datatypes::LogLevel)descriptor->FindValueByName(level_str)->number());
    result.mutable_general()->set_enabled(true);
    result.mutable_general()->set_dumpticks(false);
    result.mutable_general()->set_maxlinesize(1000);
    result.mutable_general()->set_maxfilesizemb(10.0);
    result.mutable_general()->set_pattern("[%Y-%m-%dT%H:%M:%S.%f] [%P/%t/%k] [%^%l%$] [%s:%#,%!] %v");
    return result;
}

void resetConfiguration()
{
    auto cfg = defaultConfiguration();
    setConfiguration(cfg);
}

MRA::Datatypes::LogControl getConfiguration()
{
    // Open shared memory, initialize if not existing
    int shm_fd = shm_open(_mkShmFile().c_str(), O_RDONLY, 0666);
    if (shm_fd == -1) {
        auto cfg = defaultConfiguration();
        setConfiguration(cfg);
        return cfg;
    }

    // Map shared memory
    void* shared_memory = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        close(shm_fd);
        throw std::runtime_error(std::string("Error mapping shared memory: ") + strerror(errno));
    }

    // Retrieve the serialized configuration from the shared memory buffer
    std::string serialized_config(static_cast<char*>(shared_memory));

    // Parse the serialized configuration to a LogControl object
    MRA::Datatypes::LogControl result;
    auto serialize_status = google::protobuf::util::JsonStringToMessage(serialized_config, &result);
    if (!serialize_status.ok()) {
        throw std::runtime_error(std::string("Error deserializing configuration, json_string follows\n") + serialized_config);
    }

    // Close and return
    munmap(shared_memory, SHARED_MEMORY_SIZE);
    close(shm_fd);
    return result;
}

void setConfiguration(MRA::Datatypes::LogControl const &config)
{
    LOGDEBUG("setConfiguration start");
    // Open shared memory
    int shm_fd = shm_open(_mkShmFile().c_str(), O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        throw std::runtime_error(std::string("Error opening shared memory: ") + strerror(errno));
    }
    ftruncate(shm_fd, SHARED_MEMORY_SIZE);

    // Map shared memory
    void* shared_memory = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        close(shm_fd);
        throw std::runtime_error(std::string("Error mapping shared memory: ") + strerror(errno));
    }

    // Serialize the configuration to a byte string
    std::string serialized_config;
    auto serialize_status = google::protobuf::util::MessageToJsonString(config, &serialized_config);
    if (!serialize_status.ok()) {
        throw std::runtime_error(std::string("Error serializing configuration"));
    }

    // Check if size is OK, pad with spaces to wipe any previous data which might confuse json deserialize
    int n = serialized_config.size();
    if (n > (int)SHARED_MEMORY_SIZE) {
        throw std::runtime_error(std::string("Error serializing configuration, shm size is too small (need ")
            + std::to_string(n) + ", got " + std::to_string(SHARED_MEMORY_SIZE) + " bytes)");
    }
    serialized_config += '\0';

    // Copy the serialized configuration to the shared memory buffer
    memcpy(shared_memory, serialized_config.c_str(), serialized_config.size());

    // Done
    munmap(shared_memory, SHARED_MEMORY_SIZE);
    close(shm_fd);
    LOGDEBUG("setConfiguration end (%d bytes): %s", (int)serialized_config.size(), serialized_config.c_str());
}

MRA::Datatypes::LogSpec getConfiguration(std::string const &component)
{
    MRA::Datatypes::LogControl control = getConfiguration(); // Get current configuration

    MRA::Datatypes::LogSpec result = control.general(); // Default configuration

    // Search for the component and retrieve its configuration
    for (int i = 0; i < control.overrules_size(); ++i) {
        if (control.overrules(i).component() == component) {
            result = control.overrules(i);
            break;
        }
    }

    // Fill in component name
    result.set_component(component);

    return result;
}

void setConfiguration(std::string const &component, MRA::Datatypes::LogSpec const &config)
{
    MRA::Datatypes::LogControl control = getConfiguration(); // Get current configuration

    // Search for the component and update its configuration
    for (int i = 0; i < control.overrules_size(); ++i) {
        if (control.overrules(i).component() == component) {
            *(control.mutable_overrules(i)) = config;
            break;
        }
    }

    setConfiguration(control); // Update the entire configuration
}

} // namespace MRA::Logging

