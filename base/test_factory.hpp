#ifndef _MRA_BASE_TEST_FACTORY_HPP
#define _MRA_BASE_TEST_FACTORY_HPP

#include "gtest/gtest.h"
#include "gmock/gmock.h"
using namespace ::testing;

#include "json_convert.hpp"


namespace MRA::TestFactory
{

template <typename Tc>
typename Tc::OutputType run_testvector(std::string tv_filename)
{
    // Arrange types
    auto m = Tc();
    auto input = typename Tc::InputType();
    auto params = typename Tc::ParamsType();
    auto expected_output = typename Tc::OutputType();
    auto actual_output = typename Tc::OutputType();

    // Act - load testvector
    std::string js = read_file_as_string(tv_filename);
    nlohmann::json j = nlohmann::json::parse(js);
    convert_json_to_proto(j, "Input", input);
    convert_json_to_proto(j, "Params", params);
    convert_json_to_proto(j, "Output", expected_output);

    // Act - tick
    int error_value = m.tick(input, actual_output);

    // Assert
    EXPECT_EQ(error_value, 0);
    EXPECT_EQ(convert_proto_to_json_str(actual_output), convert_proto_to_json_str(expected_output));

    // Return (for further checks) in test instance
    return actual_output;
}

}; // namespace MRA::TestFactory

#endif // _MRA_BASE_TEST_FACTORY_HPP
