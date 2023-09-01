#include "control.hpp"
#include "google/protobuf/util/json_util.h"

#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>

namespace MRA::Logging::control
{

namespace
{
    const char* SHARED_MEMORY_FILE = "/mra_logging_shared_memory";
    const char* DEFAULT_LOG_FOLDER = "/tmp/mra_logging";
    const size_t SHARED_MEMORY_SIZE = 4096;
}

std::string logFolder()
{
    MRA::Datatypes::LogControl config = getConfiguration();
    return config.folder();
}

MRA::Datatypes::LogControl defaultConfiguration()
{
    MRA::Datatypes::LogControl result;
    result.set_folder(DEFAULT_LOG_FOLDER);
    result.mutable_general()->set_level(MRA::Datatypes::LogLevel::INFO);
    result.mutable_general()->set_enabled(true);
    result.mutable_general()->set_dumpticks(false);
    result.mutable_general()->set_maxlinesize(1000);
    result.mutable_general()->set_maxfilesizemb(10.0);
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
    int shm_fd = shm_open(SHARED_MEMORY_FILE, O_RDONLY, 0666);
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
    // Open shared memory
    int shm_fd = shm_open(SHARED_MEMORY_FILE, O_CREAT | O_RDWR, 0666);
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
    serialized_config += std::string(SHARED_MEMORY_SIZE - n, ' ');

    // Copy the serialized configuration to the shared memory buffer
    memcpy(shared_memory, serialized_config.c_str(), serialized_config.size());

    // Done
    munmap(shared_memory, SHARED_MEMORY_SIZE);
    close(shm_fd);
}

MRA::Datatypes::LogSpec getConfiguration(std::string const &component)
{
    MRA::Datatypes::LogControl control = getConfiguration(); // Get current configuration

    MRA::Datatypes::LogSpec result; // Default configuration

    // Search for the component and retrieve its configuration
    for (int i = 0; i < control.overrules_size(); ++i) {
        if (control.overrules(i).component() == component) {
            result = control.overrules(i);
            break;
        }
    }

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
