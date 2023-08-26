// this file was produced by MRA-codegen.py from template_test.cpp
// with the intent of allowing user to add custom tests


// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_factory.hpp"
using namespace ::testing;

// System under test:
#include "RobotsportsProofIsAlive.hpp"
#include <cmath>
using namespace MRA;

// Basic tick shall run OK and return error_value 0.
TEST(RobotsportsProofIsAliveTest, basicTick)
{
    // Arrange
    auto m = RobotsportsProofIsAlive::RobotsportsProofIsAlive();

    // Act
    int error_value = m.tick();

    // Assert
    EXPECT_EQ(error_value, 0);
}

// helper function for adding some seconds to a google timestamp
google::protobuf::Timestamp timeFromDouble(google::protobuf::Timestamp const &t0, double dt)
{
    google::protobuf::Timestamp result = t0;
    google::protobuf::Duration duration;
    duration.set_seconds(0);
    duration.set_nanos(static_cast<int32_t>(dt * 1e9));
    return result + duration;
}

// Turn test check if all target positions are correct
TEST(RobotsportsProofIsAliveTest, turnTest)
{
    // Arrange
    auto m = RobotsportsProofIsAlive::RobotsportsProofIsAlive();
    auto input = RobotsportsProofIsAlive::Input();
    auto output = RobotsportsProofIsAlive::Output();
    auto state = RobotsportsProofIsAlive::State();
    auto local = RobotsportsProofIsAlive::Local();
    auto params = m.defaultParams();
    google::protobuf::Timestamp t0 = google::protobuf::util::TimeUtil::GetCurrentTime(); // arbitrary

    input.mutable_worldstate()->mutable_robot()->set_active(true);

    // start in middle, expect turn to left
    int error_value = m.tick(timeFromDouble(t0, 0.0), input, params, state, output, local);

    // Asserts for turn from middle to left position
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
    EXPECT_EQ(output.target().position().x(), 0.0);
    EXPECT_EQ(output.target().position().y(), 0.0);
    EXPECT_NEAR(output.target().position().rz(), params.angle_in_degrees()*(M_PI/180), 1e-4);

    // start left, expect turn to right
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_rz(output.target().position().rz());
    error_value = m.tick(timeFromDouble(t0, 1.0), input, params, state, output, local);
    // Asserts for turn from left position to right position
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
    EXPECT_EQ(output.target().position().x(), 0.0);
    EXPECT_EQ(output.target().position().y(), 0.0);
    EXPECT_NEAR(output.target().position().rz(), -params.angle_in_degrees()*(M_PI/180), 1e-4);

    // start right, expect turn to middle position (starting postion)
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_rz(output.target().position().rz());
    error_value = m.tick(timeFromDouble(t0, 2.0), input, params, state, output, local);
    // Asserts for turn from right position to middle (starting) position
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
    EXPECT_EQ(output.target().position().x(), 0.0);
    EXPECT_EQ(output.target().position().y(), 0.0);
    EXPECT_NEAR(output.target().position().rz(), 0, 1e-4);
}

// Time out test, check if time out is reported when turn took too long
TEST(RobotsportsProofIsAliveTest, timeoutTest)
{
    // Arrange
    auto m = RobotsportsProofIsAlive::RobotsportsProofIsAlive();
    auto input = RobotsportsProofIsAlive::Input();
    auto output = RobotsportsProofIsAlive::Output();
    auto state = RobotsportsProofIsAlive::State();
    auto local = RobotsportsProofIsAlive::Local();
    auto params = m.defaultParams();
    google::protobuf::Timestamp t0 = google::protobuf::util::TimeUtil::GetCurrentTime(); // arbitrary

    input.mutable_worldstate()->mutable_robot()->set_active(true);

    // start in middle, expect turn to left
    int error_value = m.tick(timeFromDouble(t0, 0.0), input, params, state, output, local);

    // Asserts for turn from middle to left position
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
    EXPECT_EQ(output.target().position().x(), 0.0);
    EXPECT_EQ(output.target().position().y(), 0.0);
    EXPECT_NEAR(output.target().position().rz(), params.angle_in_degrees()*(M_PI/180), 1e-4);

    // start left, expect turn to right
    error_value = m.tick(timeFromDouble(t0, 11.0), input, params, state, output, local);
    // Asserts for turn from left position to right position
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::FAILED);
}

// Basic tick shall run OK and return error_value 0.
TEST(RobotsportsProofIsAliveTest, robotInactiveTest)
{
    // Arrange
    auto m = RobotsportsProofIsAlive::RobotsportsProofIsAlive();
    auto input = RobotsportsProofIsAlive::Input();
    auto output = RobotsportsProofIsAlive::Output();
    input.mutable_worldstate()->mutable_robot()->set_active(false);

    // Act
    int error_value = m.tick(input, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::FAILED);
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

