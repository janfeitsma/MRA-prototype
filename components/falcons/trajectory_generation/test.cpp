// this file was produced by MRA-codegen.py from template_test.cpp
// with the intent of allowing user to add custom tests


// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_factory.hpp"
using namespace ::testing;

// System under test:
#include "FalconsTrajectoryGeneration.hpp"
using namespace MRA;

// Other includes
#include "geometry.hpp"


// Basic tick shall run OK and return error_value 0.
TEST(FalconsTrajectoryGenerationTest, basicTick)
{
    // Arrange
    auto m = FalconsTrajectoryGeneration::FalconsTrajectoryGeneration();

    // Act
    int error_value = m.tick();

    // Assert
    EXPECT_EQ(error_value, 0);
}

// X-only move
TEST(FalconsTrajectoryGenerationTest, moveX)
{
    // Arrange
    auto m = FalconsTrajectoryGeneration::FalconsTrajectoryGeneration();
    auto input = FalconsTrajectoryGeneration::Input();
    auto output = FalconsTrajectoryGeneration::Output();
    input.mutable_setpoint()->mutable_position()->set_x(2.0);
    input.mutable_worldstate()->mutable_robot()->set_active(true);

    // Act
    int error_value = m.tick(input, output);

    // Assert
    MRA::Geometry::Position final_pos(output.final().position());
    EXPECT_EQ(error_value, 0);
    EXPECT_GT(output.duration(), 2.5);
    //EXPECT_FLOAT_EQ(final_pos.x, 2.0); // for simulation use cases, a final perfectly converging tick would be nice...
    EXPECT_NEAR(final_pos.x, 2.0, 0.03); // TODO: delta is configured VC tolerance -- replace with above line
    EXPECT_FLOAT_EQ(final_pos.y, 0.0);
    EXPECT_FLOAT_EQ(final_pos.rz, 0.0);
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

