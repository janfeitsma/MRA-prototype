// this file was produced by MRA-codegen.py from template_test.cpp
// with the intent of allowing user to add custom tests


// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_factory.hpp"
using namespace ::testing;

// System under test:
#include "RobotsportsGetballIntercept.hpp"
using namespace MRA;

// Basic tick shall run OK and return error_value 0.
TEST(RobotsportsGetballInterceptTest, basicTick)
{
    // Arrange
    auto m = RobotsportsGetballIntercept::RobotsportsGetballIntercept();
    auto input = RobotsportsGetballIntercept::Input();
    auto output = RobotsportsGetballIntercept::Output();
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_x(-2.0);
    input.mutable_worldstate()->mutable_ball()->mutable_position()->set_y(-8.0);
    input.mutable_worldstate()->mutable_ball()->mutable_velocity()->set_y(1.0);

    // Act
    int error_value = m.tick();

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
    EXPECT_EQ(output.target().position().x(), 0.0);
    EXPECT_EQ(output.target().position().y(), 0.0);


}


int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

