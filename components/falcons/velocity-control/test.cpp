// this file was produced by MRA-codegen.py from template_test.cpp
// with the intent of allowing user to add custom tests


// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_factory.hpp"
using namespace ::testing;

// System under test:
#include "FalconsVelocityControl.hpp"
using namespace MRA;

// Basic tick shall run OK and return error_value 0.
TEST(FalconsVelocityControlTest, basicTick)
{
    // Arrange
    auto m = FalconsVelocityControl::FalconsVelocityControl();

    // Act
    int error_value = m.tick();

    // Assert
    EXPECT_EQ(error_value, 0);
}

// When robot is inactive, the robot shall STOP.
TEST(FalconsVelocityControlTest, robotInactive)
{
    // Arrange
    auto m = FalconsVelocityControl::FalconsVelocityControl();
    auto input = FalconsVelocityControl::Input();
    auto output = FalconsVelocityControl::Output();
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_x(1.0);
    input.mutable_setpoint()->mutable_position()->set_x(2.0);
    input.mutable_worldstate()->mutable_robot()->set_active(false);

    // Act
    int error_value = m.tick(input, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.velocity().x(), 0.0);
    EXPECT_EQ(output.velocity().y(), 0.0);
    EXPECT_EQ(output.velocity().rz(), 0.0);
}

// When no input is given, the robot shall STOP.
TEST(FalconsVelocityControlTest, nominalOutput)
{
    // Arrange
    auto m = FalconsVelocityControl::FalconsVelocityControl();
    auto input = FalconsVelocityControl::Input();
    auto output = FalconsVelocityControl::Output();

    // Act
    int error_value = m.tick();

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(output.velocity().x(), 0.0);
    EXPECT_EQ(output.velocity().y(), 0.0);
    EXPECT_EQ(output.velocity().rz(), 0.0);
}

// Section: basic moves, stateless

TEST(FalconsVelocityControlTest, moveX)
{
    // Arrange
    auto m = FalconsVelocityControl::FalconsVelocityControl();
    auto input = FalconsVelocityControl::Input();
    auto output = FalconsVelocityControl::Output();
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_x(1.0);
    input.mutable_worldstate()->mutable_robot()->mutable_velocity()->set_x(0.0);
    input.mutable_setpoint()->mutable_position()->set_x(2.0);
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    auto params = MRA::LoadDefaultParams<FalconsVelocityControl::ParamsType>("components/falcons/velocity-control/interface/DefaultParams.json");
    float acc = 1.5;
    params.mutable_limits(0)->mutable_maxacc()->set_x(acc);
    float dt = 1.0 / 40;
    params.set_dt(dt);

    // Act
    int error_value = m.tick(input, params, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_FLOAT_EQ(output.velocity().x(), acc * dt);
    EXPECT_FLOAT_EQ(output.velocity().y(), 0.0);
    EXPECT_FLOAT_EQ(output.velocity().rz(), 0.0);
}

TEST(FalconsVelocityControlTest, moveY)
{
    // Arrange
    auto m = FalconsVelocityControl::FalconsVelocityControl();
    auto input = FalconsVelocityControl::Input();
    auto output = FalconsVelocityControl::Output();
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_y(1.0);
    input.mutable_worldstate()->mutable_robot()->mutable_velocity()->set_y(0.0);
    input.mutable_setpoint()->mutable_position()->set_y(2.0);
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    auto params = MRA::LoadDefaultParams<FalconsVelocityControl::ParamsType>("components/falcons/velocity-control/interface/DefaultParams.json");
    float acc = 1.8;
    params.mutable_limits(0)->mutable_maxacc()->set_yforward(acc);
    float dt = 1.0 / 30;
    params.set_dt(dt);

    // Act
    int error_value = m.tick(input, params, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_FLOAT_EQ(output.velocity().x(), 0.0);
    EXPECT_FLOAT_EQ(output.velocity().y(), acc * dt);
    EXPECT_FLOAT_EQ(output.velocity().rz(), 0.0);
}

TEST(FalconsVelocityControlTest, moveRz)
{
    // Arrange
    auto m = FalconsVelocityControl::FalconsVelocityControl();
    auto input = FalconsVelocityControl::Input();
    auto output = FalconsVelocityControl::Output();
    input.mutable_worldstate()->mutable_robot()->mutable_position()->set_rz(1.0);
    input.mutable_worldstate()->mutable_robot()->mutable_velocity()->set_rz(0.0);
    input.mutable_setpoint()->mutable_position()->set_rz(2.0);
    input.mutable_worldstate()->mutable_robot()->set_active(true);
    auto params = MRA::LoadDefaultParams<FalconsVelocityControl::ParamsType>("components/falcons/velocity-control/interface/DefaultParams.json");
    float acc = 1.7;
    params.mutable_limits(0)->mutable_maxacc()->set_rz(acc);
    float dt = 1.0 / 35;
    params.set_dt(dt);

    // Act
    int error_value = m.tick(input, params, output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_FLOAT_EQ(output.velocity().x(), 0.0);
    EXPECT_FLOAT_EQ(output.velocity().y(), 0.0);
    EXPECT_FLOAT_EQ(output.velocity().rz(), acc * dt);
}

int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

