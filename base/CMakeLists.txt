include_directories(.)

add_library(MRA-base
    json_convert.cpp
)
target_link_libraries(MRA-base nlohmann_json::nlohmann_json)


