#include "control.hpp"
#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cerrno>

namespace MRA::Logging
{

namespace
{
    const char* SHARED_MEMORY_FILE = "/mra_logging_shared_memory";
    const size_t SHARED_MEMORY_SIZE = sizeof(MRA::Datatypes::LogControl);
}

std::string logFolder()
{
    MRA::Datatypes::LogControl config = getConfiguration();
    return config.folder();
}

MRA::Datatypes::LogControl getConfiguration()
{
    int shm_fd = shm_open(SHARED_MEMORY_FILE, O_RDONLY, 0666);
    if (shm_fd == -1) {
        std::cerr << "Error opening shared memory: " << strerror(errno) << std::endl;
        // Handle error, return default configuration or throw an exception
    }
    
    void* shared_memory = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        std::cerr << "Error mapping shared memory: " << strerror(errno) << std::endl;
        close(shm_fd);
        // Handle error, return default configuration or throw an exception
    }

    MRA::Datatypes::LogControl* config = static_cast<MRA::Datatypes::LogControl*>(shared_memory);
    MRA::Datatypes::LogControl result = *config;

    munmap(shared_memory, SHARED_MEMORY_SIZE);
    close(shm_fd);

    return result;
}

void setConfiguration(MRA::Datatypes::LogControl const &config)
{
    int shm_fd = shm_open(SHARED_MEMORY_FILE, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        std::cerr << "Error opening shared memory: " << strerror(errno) << std::endl;
        // Handle error
    }

    ftruncate(shm_fd, SHARED_MEMORY_SIZE);

    void* shared_memory = mmap(NULL, SHARED_MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_memory == MAP_FAILED) {
        std::cerr << "Error mapping shared memory: " << strerror(errno) << std::endl;
        close(shm_fd);
        // Handle error
    }

    MRA::Datatypes::LogControl* config_ptr = static_cast<MRA::Datatypes::LogControl*>(shared_memory);
    *config_ptr = config;

    munmap(shared_memory, SHARED_MEMORY_SIZE);
    close(shm_fd);
}

void setConfiguration(std::string const &component, MRA::Datatypes::LogSpec const &config)
{
    // Implement the setConfiguration function for a specific component
    // using shared memory
}

} // namespace MRA::Logging

