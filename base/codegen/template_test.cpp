// CODEGEN_NOTE
// with the intent of allowing user to add custom tests


// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_factory.hpp"
using namespace ::testing;

// System under test:
#include "COMPONENT_CPP_NAME_CAMELCASE.hpp"


// Basic tick shall run OK and return error_value 0.
TEST(COMPONENT_CPP_NAME_CAMELCASETest, basicTick)
{
    // Arrange
    auto m = COMPONENT_CPP_NAME_CAMELCASE::COMPONENT_CPP_NAME_CAMELCASE();

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

