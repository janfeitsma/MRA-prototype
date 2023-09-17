// Test suite for MRA logger.

// Include testframework and system includes
#include "test_mra_logger_helpers.hpp"
#include <filesystem>
#include <stdlib.h>

// System under test
#include "logging.hpp"

// Auxiliary component (rather than defining a dummy component)
#include "RobotsportsProofIsAlive.hpp"


// Helper function: manipulate logger to run independent of everything else, via environment
void configure_logger() {
    setenv("MRA_LOGGER_CONTEXT", "unittest", 1);
}

// Helper function: run a single tick
void runtick() {
    auto m = MRA::RobotsportsProofIsAlive::RobotsportsProofIsAlive();
    int error_value = m.tick();
    EXPECT_EQ(error_value, 0);
}

// Default tick shall produce logging
TEST_F(TestFixture, defaultLogging) {
    // Act
    runtick();

    // Assert
    std::string expected_log_file = LOG_FOLDER_TEST "/RobotsportsProofIsAlive.log";
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

/* TODO: doesnt work yet
// Configurable log level and coverage for all levels
TEST_F(TestFixture, configurableLogLevels) {
    // Arrange
    std::string expected_log_file = LOG_FOLDER_TEST "/RobotsportsProofIsAlive.log";
    auto cfg = MRA::Logging::control::getConfiguration(); // return type: Logging.proto
    cfg.mutable_general()->set_enabled(true);
    MRA::Logging::control::setConfiguration(cfg);
    std::cout << "TESTCFG " << MRA::convert_proto_to_json_str(MRA::Logging::control::getConfiguration()) << std::endl;

    // Act
    MRA_LOG_CRITICAL("test critical %d", 37);
    MRA_LOG_ERROR("test error %d", 37);
    MRA_LOG_WARNING("test warning %d", 37);
    MRA_LOG_INFO("test info %d", 37);
    MRA_LOG_DEBUG("test debug %d", 37);

    sleep(3);
    // Assert
    EXPECT_EQ(count_log_lines(expected_log_file), 5);
}*/

int main(int argc, char **argv) {
    configure_logger();
    testing::InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

