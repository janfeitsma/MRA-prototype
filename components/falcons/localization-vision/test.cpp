// this file was produced by MRA-codegen.py from template_test.cpp
// with the intent of allowing user to add custom tests


// Include testframework
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "test_factory.hpp"
using namespace ::testing;

// Other includes
#include <fstream>

// System under test:
#include "FalconsLocalizationVision.hpp"
using namespace MRA;

// Basic tick shall run OK and return error_value 0.
TEST(FalconsLocalizationVisionTest, basicTick)
{
    // Arrange
    auto m = FalconsLocalizationVision::FalconsLocalizationVision();

    // Act
    int error_value = m.tick();

    // Assert
    EXPECT_EQ(error_value, 0);
}

// Some pixels
TEST(FalconsLocalizationVisionTest, somePixels)
{
    // Arrange
    auto m = FalconsLocalizationVision::FalconsLocalizationVision();
    auto input = FalconsLocalizationVision::Input();
    auto output = FalconsLocalizationVision::Output();
    auto state = FalconsLocalizationVision::State();
    auto local = FalconsLocalizationVision::Local();
    auto params = m.defaultParams();
    FalconsLocalizationVision::Landmark p;
    p.set_x(3); p.set_y(3); *input.add_landmarks() = p;
    p.set_x(3); p.set_y(4); *input.add_landmarks() = p;
    p.set_x(4); p.set_y(3); *input.add_landmarks() = p;
    params.set_debug(true);

    // Act
    int error_value = m.tick(input, params, state, output, local);

    // debug dump
    if (params.debug())
    {
        std::ofstream dumpState;
        dumpState.open("/tmp/locState.bin");
        local.fitresultfloor().SerializeToOstream(&dumpState);
        dumpState.close();
    }

    // Assert
    EXPECT_EQ(error_value, 0);
}


int main(int argc, char **argv)
{
    // Verify that the version of the library that we linked against is
    // compatible with the version of the headers we compiled against.
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    InitGoogleTest(&argc, argv);
    int r = RUN_ALL_TESTS();
    return r;
}

