// this file was produced by MRA-codegen.py from template_test.cpp
// with the intent of allowing user to add custom tests


// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_factory.hpp"
using namespace ::testing;

// System under test:
#include "FalconsGetballFetch.hpp"
using namespace MRA;



// Basic tick shall run OK and return error_value 0.
TEST(FalconsGetballFetchTest, basicTick)
{
    // Arrange
    auto m = FalconsGetballFetch::FalconsGetballFetch();

    // Act
    int error_value = m.tick();

    // Assert
    EXPECT_EQ(error_value, 0);
}

// When robot is inactive, the action shall fail.
TEST(FalconsGetballFetchTest, robotInactive)
{
    // Arrange
    auto m = FalconsGetballFetch::FalconsGetballFetch();
    auto input = FalconsGetballFetch::Input();
    auto output = FalconsGetballFetch::Output();
    input.mutable_worldstate()->mutable_robot()->set_active(false);

    // Act
    int error_value = m.tick(input, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::FAILED);
}

// Move towards stationary ball in positive x direction.
TEST(FalconsGetballFetchTest, getStationaryBall)
{
    // Arrange
    auto m = FalconsGetballFetch::FalconsGetballFetch();
    auto input = FalconsGetballFetch::Input();
    auto output = FalconsGetballFetch::Output();
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_x(-2.0);
    input.mutable_worldstate()->mutable_ball()->mutable_position()->set_x(2.0);

    // Act
    int error_value = m.tick(input, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
    EXPECT_EQ(output.target().position().x(), 2.0);
}

// When robot has the ball, the action PASSED.
TEST(FalconsGetballFetchTest, hasBallPassed)
{
    // Arrange
    auto m = FalconsGetballFetch::FalconsGetballFetch();
    auto input = FalconsGetballFetch::Input();
    auto output = FalconsGetballFetch::Output();
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    input.mutable_worldstate()->mutable_robot()->set_hasball(true);

    // Act
    int error_value = m.tick(input, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::PASSED);
    // TODO: what do we require on target? target==current or not? prevent move. Current: target shall be empty
    std::string json_output;
    EXPECT_TRUE(google::protobuf::util::MessageToJsonString(output, &json_output).ok());
    EXPECT_EQ(json_output, "{\"actionresult\":\"PASSED\"}");
}

// Match setup, full/realistic data, kickoff-prepare.
TEST(FalconsGetballFetchTest, matchKickoff)
{
    // A test vector contains Input, Output, Params
    // The factory will run a tick with provided data and compare against expected output
    auto output = TestFactory::run_testvector<FalconsGetballFetch::FalconsGetballFetch>(std::string("components/falcons/getball_fetch/testdata/kickoff_prepare.json"));

    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

