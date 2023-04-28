// this file was produced by MRA-codegen.py from template_test.cpp
// with the intent of allowing user to add custom tests


// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_factory.hpp"
using namespace ::testing;

// System under test:
#include "FalconsGetball.hpp"
using namespace MRA;


// Basic tick shall run OK and return error_value 0.
TEST(FalconsGetballTest, basicTick)
{
    // Arrange
    auto m = FalconsGetball::FalconsGetball();

    // Act
    int error_value = m.tick();

    // Assert
    EXPECT_EQ(error_value, 0);
}


int main(int argc, char **argv)
{
    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

