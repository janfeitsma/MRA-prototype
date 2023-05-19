cmake_minimum_required(VERSION 3.5.1)

##########################################################
# general configuration
##########################################################

project(MRA LANGUAGES CXX)

# use C++17 standard, or suffer errors re. using std::filesystem
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror")

# dependency: ProtoBuf
find_package(Protobuf REQUIRED)
find_library(PROTOBUF_LIBRARY protobuf)

# dependency: json
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)


##########################################################
# common datatypes
##########################################################

set(MESSAGE_PROTOS_COMMON
    datatypes/ActionResult.proto
    datatypes/Pose.proto
    datatypes/Ball.proto
    datatypes/Obstacle.proto
    datatypes/Player.proto
    datatypes/Target.proto
    datatypes/WorldState.proto
)

add_library(MRA-datatypes)
target_include_directories(MRA-datatypes PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(MRA-datatypes protobuf)

protobuf_generate(TARGET MRA-datatypes PROTOS ${MESSAGE_PROTOS_COMMON} LANGUAGE cpp IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR})


##########################################################
# common libraries
##########################################################

add_library(MRA-libraries
    libraries/geometry/geometry.cpp
)
target_include_directories(MRA-datatypes PUBLIC libraries/geometry)
target_link_libraries(MRA-libraries MRA-datatypes)


##########################################################
# base headers / utilities
##########################################################

include_directories(base)


##########################################################
# component interfaces
##########################################################

set(MESSAGE_PROTOS_COMPONENTS
    components/falcons/getball-fetch/interface/Input.proto
    components/falcons/getball-fetch/interface/Params.proto
    components/falcons/getball-fetch/interface/Output.proto
    components/robotsports/getball-fetch/interface/Input.proto
    components/robotsports/getball-fetch/interface/Params.proto
    components/robotsports/getball-fetch/interface/Output.proto
)

add_library(MRA-components-proto)
target_include_directories(MRA-components-proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(MRA-components-proto MRA-datatypes protobuf)

protobuf_generate(TARGET MRA-components-proto PROTOS ${MESSAGE_PROTOS_COMPONENTS} LANGUAGE cpp IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR})


##########################################################
# component implementations
##########################################################

add_library(MRA-components
    components/falcons/getball-fetch/tick.cpp
    components/robotsports/getball-fetch/tick.cpp
)
target_link_libraries(MRA-components MRA-libraries MRA-components-proto nlohmann_json::nlohmann_json)

