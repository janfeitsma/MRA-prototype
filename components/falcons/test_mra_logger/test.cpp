// Test suite for MRA logger.

// Include testframework and system includes
#include "src/test_helpers.hpp"
#include <filesystem>
#include <stdlib.h>

// System under test
#include "logging.hpp"
#include "FalconsTestMraLogger.hpp"

std::string EXPECTED_LOG_FILE = LOG_FOLDER_TEST "/FalconsTestMraLogger.log";


// Helper function: manipulate logger to run independent of everything else, via environment
void configure_logger() {
    setenv("MRA_LOGGER_CONTEXT", "unittest", 1);
}

// Helper functions: run a single tick
void runtick_with_logmessages() {
    auto m = MRA::FalconsTestMraLogger::FalconsTestMraLogger();
    MRA::FalconsTestMraLogger::InputType input;
    input.set_generatecriticalmessage(true);
    input.set_generateerrormessage(true);
    input.set_generatewarningmessage(true);
    input.set_generateinfomessage(true);
    input.set_generatedebugmessage(true);
    MRA::FalconsTestMraLogger::ParamsType params;
    MRA::FalconsTestMraLogger::OutputType output;
    int error_value = m.tick(input, params, output);
    EXPECT_EQ(error_value, 0);
}

void runtick() {
    auto m = MRA::FalconsTestMraLogger::FalconsTestMraLogger();
    int error_value = m.tick();
    EXPECT_EQ(error_value, 0);
}

// Default tick shall produce logging
TEST_F(TestFixture, defaultLogging) {
    // Act
    runtick();

    // Assert
    EXPECT_TRUE(check_log_folder_existing());
    EXPECT_EQ(count_log_files(), 1);
    EXPECT_TRUE(std::filesystem::exists(EXPECTED_LOG_FILE));
    EXPECT_GT(count_log_lines(EXPECTED_LOG_FILE), 0);
}

// Produce all sorts of logging, test for configurable level
TEST_F(TestFixture, logLevelCritical) {
    // Arrange
    auto cfg = MRA::Logging::control::getConfiguration();
    cfg.mutable_general()->set_level(MRA::Datatypes::CRITICAL);
    MRA::Logging::control::setConfiguration(cfg);

    // Act
    runtick_with_logmessages();

    // Assert
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[critical]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[error]"), 0);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[warning]"), 0);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[info]"), 0);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[debug]"), 0);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[trace]"), 0);
}

TEST_F(TestFixture, logLevelError) {
    // Arrange
    auto cfg = MRA::Logging::control::getConfiguration();
    cfg.mutable_general()->set_level(MRA::Datatypes::ERROR);
    MRA::Logging::control::setConfiguration(cfg);

    // Act
    runtick_with_logmessages();

    // Assert
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[critical]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[error]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[warning]"), 0);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[info]"), 0);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[debug]"), 0);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[trace]"), 0);
}

TEST_F(TestFixture, logLevelWarning) {
    // Arrange
    auto cfg = MRA::Logging::control::getConfiguration();
    cfg.mutable_general()->set_level(MRA::Datatypes::WARNING);
    MRA::Logging::control::setConfiguration(cfg);

    // Act
    runtick_with_logmessages();

    // Assert
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[critical]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[error]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[warning]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[info]"), 0);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[debug]"), 0);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[trace]"), 0);
}

TEST_F(TestFixture, logLevelInfo) {
    // Arrange
    auto cfg = MRA::Logging::control::getConfiguration();
    cfg.mutable_general()->set_level(MRA::Datatypes::INFO);
    MRA::Logging::control::setConfiguration(cfg);

    // Act
    runtick_with_logmessages();

    // Assert
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[critical]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[error]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[warning]"), 1);
    EXPECT_GE(log_content_count_substring(EXPECTED_LOG_FILE, "[info]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[debug]"), 0);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[trace]"), 0);
}

TEST_F(TestFixture, logLevelDebug) {
    // Arrange
    auto cfg = MRA::Logging::control::getConfiguration();
    cfg.mutable_general()->set_level(MRA::Datatypes::DEBUG);
    MRA::Logging::control::setConfiguration(cfg);

    // Act
    runtick_with_logmessages();

    // Assert
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[critical]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[error]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[warning]"), 1);
    EXPECT_GE(log_content_count_substring(EXPECTED_LOG_FILE, "[info]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[debug]"), 1);
    EXPECT_EQ(log_content_count_substring(EXPECTED_LOG_FILE, "[trace]"), 0);
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
    configure_logger();
    testing::InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

