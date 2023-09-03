// Test suite for MRA logger.

// Include testframework and system includes
#include "gtest/gtest.h"
#include <filesystem>

// System under test
#include "logging.hpp"

// Auxiliary component (rather than defining a dummy component)
#include "RobotsportsProofIsAlive.hpp"

// Mock for MRA::Logging::logFolder
namespace MRA::Logging {
    std::string logFolder() {
        return "/tmp/test_mra_logging";
    }
}

// Define for log folder
#define LOG_FOLDER "/tmp/test_mra_logging"

// Pre-test cleanup
class TestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        cleanupLogFolder();
    }

    void TearDown() override {
        cleanupLogFolder();
    }

    void cleanupLogFolder() {
        std::string logFolderPath = LOG_FOLDER;
        // Check if the directory exists
        if (std::filesystem::exists(logFolderPath)) {
            // Remove all files and subdirectories within the log folder
            for (const auto &entry : std::filesystem::directory_iterator(logFolderPath)) {
                if (entry.is_regular_file() || entry.is_directory()) {
                    std::cout << "std::filesystem::remove_all(" << entry.path() << ")";
                    //std::filesystem::remove_all(entry.path());
                }
            }
        }
    }
};

// Helper function: run a single tick
void runtick() {
    auto m = MRA::RobotsportsProofIsAlive::RobotsportsProofIsAlive();
    int error_value = m.tick();
    EXPECT_EQ(error_value, 0);
}

// Helper function: check if log folder exists
bool check_log_folder_existing(std::string log_folder = LOG_FOLDER) {
    if (!std::filesystem::exists(log_folder)) return false;
    return std::filesystem::is_directory(log_folder);
}

// Helper function: count number of produced log files
int count_log_files(std::string log_folder = LOG_FOLDER) {
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

// Default tick shall produce logging
TEST_F(TestFixture, defaultLogging) {
    MRA::Logging::control::resetConfiguration();
    // Act
    runtick();

    // Assert
    std::string expected_log_file = LOG_FOLDER "/robotsports/proof_is_alive.log";
    EXPECT_TRUE(check_log_folder_existing());
    EXPECT_EQ(count_log_files(), 1);
    EXPECT_TRUE(std::filesystem::exists(expected_log_file));
    EXPECT_GT(count_log_lines(expected_log_file), 0);
}

// If so configured, then do not produce logging
TEST_F(TestFixture, allowDisableLogging) {
    // Arrange
    auto cfg = MRA::Logging::control::getConfiguration(); // return type: Logging.proto
    cfg.mutable_general()->set_enabled(false);
    MRA::Logging::control::setConfiguration(cfg);

    // Act
    runtick();

    // Assert
    EXPECT_FALSE(check_log_folder_existing());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

