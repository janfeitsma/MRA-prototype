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

// Basic tick shall run OK and return error_value 0.
TEST(RobotsportsProofIsAliveTest, turnTest)
{
    // Arrange
    auto m = RobotsportsProofIsAlive::RobotsportsProofIsAlive();
    auto input = RobotsportsProofIsAlive::Input();
    auto output = RobotsportsProofIsAlive::Output();
    auto state = RobotsportsProofIsAlive::State();
    auto local = RobotsportsProofIsAlive::Local();
    auto params = m.defaultParams();

    input.mutable_worldstate()->mutable_robot()->set_active(true);

    // Act
    int error_value = m.tick(0.0, input, params, state, output, local);


    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.actionresult(), MRA::Datatypes::RUNNING);
    EXPECT_EQ(output.target().position().x(), 0.0);
    EXPECT_EQ(output.target().position().y(), 0.0);
    EXPECT_NEAR(output.target().position().rz(), params.angle_in_degrees()*(M_PI/180), 1e-4);

}



int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

