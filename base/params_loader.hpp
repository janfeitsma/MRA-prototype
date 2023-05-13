#ifndef _MRA_BASE_PARAMS_LOADER_HPP
#define _MRA_BASE_PARAMS_LOADER_HPP

#include "json_convert.hpp"
#include <filesystem>

namespace MRA
{

template <typename Tp>
Tp LoadDefaultParams(std::string params_json_filename)
{
    Tp result;
    if (!std::filesystem::exists(params_json_filename))
    {
        // allow omitting defaults
        return result;
    }
    std::string js = read_file_as_string(params_json_filename);
    nlohmann::json j = nlohmann::json::parse(js);
    auto r = google::protobuf::util::JsonStringToMessage(js, &result);
    if (!r.ok())
    {
        throw std::runtime_error("failed to convert json from file " + params_json_filename + " to protobuf Params struct");
    }
    return result;
}

} // namespace MRA

#endif // _MRA_BASE_PARAMS_LOADER_HPP

