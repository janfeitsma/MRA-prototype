// this file was produced by MRA-codegen.py from template_test.cpp
// with the intent of allowing user to add custom tests


// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_factory.hpp"
using namespace ::testing;

// System under test:
#include "RobotsportsGetballFetch.hpp"
using namespace MRA;



// Basic tick shall run OK and return error_value 0.
TEST(RobotspotsGetballFetchTest, basicTick)
{
    // Arrange
    auto m = RobotsportsGetballFetch::RobotsportsGetballFetch();

    // Act
    int error_value = m.tick();

    // Assert
    EXPECT_EQ(error_value, 0);
}

// When robot is inactive, the action shall fail.
TEST(RobotsportsGetballFetchTest, robotInactive)
{
    // Arrange
    auto m = RobotsportsGetballFetch::RobotsportsGetballFetch();
    auto input = RobotsportsGetballFetch::Input();
    auto output = RobotsportsGetballFetch::Output();
    input.mutable_worldstate()->mutable_robot()->set_active(false);

    // Act
    int error_value = m.tick(input, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::FAILED);
}

// Move towards stationary ball in positive x direction.
TEST(RobotsportsGetballFetchTest, getStationaryBall)
{
    // Arrange
    auto m = RobotsportsGetballFetch::RobotsportsGetballFetch();
    auto input = RobotsportsGetballFetch::Input();
    auto output = RobotsportsGetballFetch::Output();
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

// Move towards stationary ball in positive x and y direction.
TEST(RobotsportsGetballFetchTest, getStationaryBall_xy)
{
    // Arrange
    auto m = RobotsportsGetballFetch::RobotsportsGetballFetch();
    auto input = RobotsportsGetballFetch::Input();
    auto output = RobotsportsGetballFetch::Output();
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_x(-2.0);
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_y(-2.0);
    input.mutable_worldstate()->mutable_ball()->mutable_position()->set_x(2.0);
    input.mutable_worldstate()->mutable_ball()->mutable_position()->set_y(0.1);

    // Act
    int error_value = m.tick(input, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
    EXPECT_NEAR(output.target().position().x(), 2.0, 1e-4);
    EXPECT_NEAR(output.target().position().y(), 0.1, 1e-4);
    EXPECT_NEAR(output.target().position().rz(), -1.08734, 1e-4);
}


// When robot has the ball, the action PASSED.
TEST(RobotsportsGetballFetchTest, hasBallPassed)
{
    // Arrange
    auto m = RobotsportsGetballFetch::RobotsportsGetballFetch();
    auto input = RobotsportsGetballFetch::Input();
    auto output = RobotsportsGetballFetch::Output();
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
TEST(RobotsportsGetballFetchTest, matchKickoff)
{
    // A test vector contains Input, Output, Params
    // The factory will run a tick with provided data and compare against expected output
    auto output = TestFactory::run_testvector<RobotsportsGetballFetch::RobotsportsGetballFetch>(std::string("components/robotsports/getball_fetch/testdata/kickoff_prepare.json"));

    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

