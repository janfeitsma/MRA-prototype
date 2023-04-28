#ifndef _MRA_BASE_TEST_FACTORY_HPP
#define _MRA_BASE_TEST_FACTORY_HPP

#include "gtest/gtest.h"
#include "gmock/gmock.h"
using namespace ::testing;

#include <nlohmann/json.hpp>
#include <google/protobuf/util/json_util.h>
#include <filesystem>
#include <fstream>
#include <iostream>


namespace MRA::TestFactory
{

std::string read_filename_as_string(std::string filename)
{
    std::ifstream infile(filename);
    if (!infile)
    {
        std::filesystem::path cwd = std::filesystem::current_path();
        throw std::runtime_error("file " + filename + " not found at " + std::string(cwd));
    }
    std::ostringstream sstr;
    sstr << infile.rdbuf();
    return sstr.str();
}

template <typename T>
std::string convert_proto_to_json_str(T const &tproto)
{
    std::string result;
    auto r = google::protobuf::util::MessageToJsonString(tproto, &result);
    if (!r.ok())
    {
        throw std::runtime_error("failed to convert protobuf struct to json");
    }
    return result;
}

template <typename T>
void convert_json_to_proto(nlohmann::json const &j, std::string key, T &tproto)
{
    if (j.contains(key))
    {
        // data in the test vector file is either a file reference of convertable json data
        if (j[key].is_string())
        {
            // assume file reference, load
            auto r = google::protobuf::util::JsonStringToMessage(read_filename_as_string(j[key]), &tproto);
            if (!r.ok())
            {
                throw std::runtime_error("failed to convert json from file " + std::string(j[key]) + " to protobuf struct");
            }
        }
        else
        {
            // convert json value (via string)
            std::string tmp_str = nlohmann::to_string(j[key]);
            auto r = google::protobuf::util::JsonStringToMessage(tmp_str, &tproto);
            if (!r.ok())
            {
                throw std::runtime_error("failed to convert tmp json string to protobuf " + key + " struct: '" + tmp_str + "'");
            }
        }
    }
}

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
    std::string js = read_filename_as_string(tv_filename);
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
