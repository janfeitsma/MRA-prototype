#ifndef _MRA_TEST_MRA_LOGGER_HELPERS_HPP
#define _MRA_TEST_MRA_LOGGER_HELPERS_HPP

#include "logging.hpp"
#include "gtest/gtest.h"
#include <filesystem>


// Mock/manipulate MRA::Logging::logFolder, leave default intact
#define LOG_FOLDER_TEST "/tmp/unittest_mra_logging"

// Helper function: test-specific configuration
// (to not affect standard log folder, to enable hotflush)
MRA::Datatypes::LogControl testConfiguration();

// Pre-test cleanup and configuration
class TestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // switch to test config&folder
        MRA::Logging::control::setConfiguration(testConfiguration());
        cleanupLogFolder();
    }

    void TearDown() override {
        // switch back to standard config&folder
        cleanupLogFolder();
        MRA::Logging::control::resetConfiguration();
        MRA::Logging::backend::clear();
    }

    void cleanupLogFolder() {
        std::string logFolderPath = LOG_FOLDER_TEST;
        // Check if the directory exists
        if (std::filesystem::exists(logFolderPath)) {
            std::filesystem::remove_all(logFolderPath);
        }
    }
};

// Helper function: run a single tick
void runtick();

// Helper function: check if log folder exists
bool check_log_folder_existing(std::string log_folder = LOG_FOLDER_TEST);

// Helper function: count number of produced log files
int count_log_files(std::string log_folder = LOG_FOLDER_TEST);

// Helper function: count number of produced log lines
int count_log_lines(std::string filename);


#endif // #ifndef _MRA_TEST_MRA_LOGGER_HELPERS_HPP

