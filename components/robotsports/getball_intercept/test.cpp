// this file was produced by MRA-codegen.py from template_test.cpp
// with the intent of allowing user to add custom tests


// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_factory.hpp"
using namespace ::testing;
#include <cmath>

// System under test:
#include "RobotsportsGetballIntercept.hpp"
using namespace MRA;

// Basic tick shall run OK and return error_value 0.
TEST(RobotsportsGetballInterceptTest, basicTick)
{
    // Arrange
    auto m = RobotsportsGetballIntercept::RobotsportsGetballIntercept();

    // Act
    int error_value = m.tick();

    // Assert
    EXPECT_EQ(error_value, 0);
}


// test simple intercept
TEST(RobotsportsGetballInterceptTest, simpleIntercept)
{
    // Arrange
    auto m = RobotsportsGetballIntercept::RobotsportsGetballIntercept();
    auto input = RobotsportsGetballIntercept::Input();
    auto output = RobotsportsGetballIntercept::Output();
    auto params = RobotsportsGetballIntercept::Params();
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_x(-1.0);
    input.mutable_worldstate()->mutable_ball()->mutable_position()->set_x(-0.05);
    input.mutable_worldstate()->mutable_ball()->mutable_position()->set_y(-8.0);
    input.mutable_worldstate()->mutable_ball()->mutable_velocity()->set_y(1.0);
    
    params.set_actionradius(2.0);

    // Act
    int error_value = m.tick(input, params, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
    EXPECT_NEAR(output.target().position().x(), -0.05, 1e-2);
    EXPECT_NEAR(output.target().position().y(), 0.0, 1e-2);
    EXPECT_NEAR(output.target().position().rz(), -M_PI, 1e-2);
}

// test intercept with interception position too far from robot
TEST(RobotsportsGetballInterceptTest, intercept_outside_actionradius)
{
    // Arrange
    auto m = RobotsportsGetballIntercept::RobotsportsGetballIntercept();
    auto input = RobotsportsGetballIntercept::Input();
    auto output = RobotsportsGetballIntercept::Output();
    auto params = RobotsportsGetballIntercept::Params();
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_x(-1.0);
    input.mutable_worldstate()->mutable_ball()->mutable_position()->set_x(-0.05);
    input.mutable_worldstate()->mutable_ball()->mutable_position()->set_y(-8.0);
    input.mutable_worldstate()->mutable_ball()->mutable_velocity()->set_y(1.0);
    
    params.set_actionradius(0.1);  // small action radius

    // Act
    int error_value = m.tick(input, params, output);

    // Assert
    // expect starting position    
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::FAILED);
    EXPECT_NEAR(output.target().position().x(), -1.0, 1e-2);
    EXPECT_NEAR(output.target().position().y(), 0.0, 1e-2);
    EXPECT_NEAR(output.target().position().rz(), 0, 1e-2);
}


int main(int argc, char **argv)
{
	setenv("MRA_LOGGER_CONTEXT", "unittest", 1);
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

