#ifndef _MRA_BASE_JSON_CONVERT_HPP
#define _MRA_BASE_JSON_CONVERT_HPP

#include "nlohmann/json.hpp"
#include "google/protobuf/util/json_util.h"


namespace MRA
{

std::string read_file_as_string(std::string filename);

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

template <typename T> // TODO: rework this one, it isn't pretty and too magic.
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

// partial update based on json object, to cover missing functionality/option in google::protobuf::util::JsonStringToMessage
// when JsonStringToMessage applies json string "{}", then the message gets reset ...
// it would be nice to have an option 'merge=true' or something
// this template achieves exactly that, it would leave the message unaltered when given an empty json object
template <typename T>
void merge_json_into_proto(nlohmann::json const &j, T &tproto)
{
    // convert proto message to json string
    std::string proto_json_str;
    auto r = google::protobuf::util::MessageToJsonString(tproto, &proto_json_str);
    if (!r.ok())
    {
        throw std::runtime_error("failed to protobuf message to json string");
    }
    // convert json string to json object
    nlohmann::json jm = nlohmann::json::parse(proto_json_str);
    // merge given json object
    bool merge = true;
    jm.update(j, merge);
    // convert merged json string back to proto message
    r = google::protobuf::util::JsonStringToMessage(nlohmann::to_string(jm), &tproto);
    if (!r.ok())
    {
        throw std::runtime_error("failed to convert merged json string to protobuf message");
    }
}

template <typename T>
void merge_json_into_proto(std::string const &s, T &tproto)
{
    nlohmann::json j = nlohmann::json::parse(s);
    merge_json_into_proto(j, tproto);
}

template <typename T>
void merge_json_into_proto(char const *s, T &tproto)
{
    merge_json_into_proto(std::string(s), tproto);
}

}; // namespace MRA

#endif // _MRA_BASE_JSON_CONVERT_HPP
