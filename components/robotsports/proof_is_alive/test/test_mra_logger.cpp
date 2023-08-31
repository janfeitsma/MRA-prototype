// Test suite for MRA logger.

// Include testframework
#include "gtest/gtest.h"

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
bool check_log_folder_existing() {
    // TODO: Check if log folder exists
    return true;
}

// Default tick shall produce logging
TEST_F(TestFixture, defaultLogging) {
    MRA::Logging::resetConfiguration();
    // Act
    runtick();

    // Assert
    std::string expected_log_file = LOG_FOLDER "/robotsports/proof_is_alive.log";
    // TODO: Check if log file exists, is fresh, and not empty
}

// If so configured, then do not produce logging
TEST_F(TestFixture, allowDisableLogging) {
    // Arrange
    auto cfg = MRA::Logging::getConfiguration(); // return type: Logging.proto
    cfg.mutable_general()->set_tofile(false);
    MRA::Logging::setConfiguration(cfg);

    // Act
    runtick();

    // Assert
    // TODO: Check if log folder was not created at all
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

