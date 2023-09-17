#include "test_helpers.hpp"

#include <fstream>
#include <filesystem>


// Helper function: test-specific configuration
// (to not affect standard log folder, to enable hotflush)
MRA::Datatypes::LogControl testConfiguration() {
    MRA::Datatypes::LogControl result = MRA::Logging::control::defaultConfiguration();
    result.set_folder(LOG_FOLDER_TEST);
    result.set_filename("<maincomponent>.log");
    result.mutable_general()->set_component("MRA-test");
    result.mutable_general()->set_level(MRA::Datatypes::LogLevel::INFO);
    result.mutable_general()->set_enabled(true);
    result.mutable_general()->set_dumpticks(false);
    result.mutable_general()->set_maxlinesize(1000);
    result.mutable_general()->set_maxfilesizemb(10.0);
    result.mutable_general()->set_pattern("[%Y-%m-%dT%H:%M:%S.%f] [%n] [%^%l%$] %v");
    result.mutable_general()->set_hotflush(true);
    return result;
}

// Helper function: check if log folder exists
bool check_log_folder_existing(std::string log_folder) {
    if (!std::filesystem::exists(log_folder)) return false;
    return std::filesystem::is_directory(log_folder);
}

// Helper function: count number of produced log files
int count_log_files(std::string log_folder) {
    try {
        int count = 0;
        // Use std::filesystem to iterate through files in the log folder
        for (const auto &entry : std::filesystem::directory_iterator(log_folder)) {
            if (entry.is_regular_file()) {
                count++;
            }
        }
        return count;
    } catch (const std::filesystem::filesystem_error &e) {
        // Handle any errors that might occur during folder iteration
        return -1; // Return -1 in case of any error
    }
}

// Helper function: count number of produced log lines
int count_log_lines(std::string filename) {
    std::ifstream fh(filename);
    int count = 0;
    if (fh.is_open()) {
        std::string line;
        while (std::getline(fh, line)) {
            count++;
        }
        fh.close();
    }
    return count;
}

