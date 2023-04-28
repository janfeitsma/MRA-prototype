#ifndef _MRA_BASE_JSON_CONVERT_HPP
#define _MRA_BASE_JSON_CONVERT_HPP

#include <nlohmann/json.hpp>
#include <google/protobuf/util/json_util.h>
#include <filesystem>
#include <fstream>
#include <iostream>


namespace MRA
{

std::string read_file_as_string(std::string filename)
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
            auto r = google::protobuf::util::JsonStringToMessage(read_file_as_string(j[key]), &tproto);
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

}; // namespace MRA

#endif // _MRA_BASE_JSON_CONVERT_HPP
