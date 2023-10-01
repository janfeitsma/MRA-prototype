cmake_minimum_required(VERSION 3.5.1)

##########################################################
# general configuration
##########################################################

project(MRA LANGUAGES CXX)

# use C++17 standard, or suffer errors re. using std::filesystem
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror -Wno-address -Wno-reorder -Wno-array-bounds")
 # to prevent compiler errors added the not preferred flags: -Wno-address -Wno-reorder -Wno-array-bounds

# dependency: ProtoBuf
# check if ProtoBuf is installed
# install on Ubuntu: sudo apt-get install protobuf-compiler
find_package(Protobuf REQUIRED)
if(NOT PROTOBUF_FOUND)
message(FATAL_ERROR, "Protobuf could not be find. Install protobuf (e.g. sudo apt-get install protobuf-compiler)")
endif(NOT PROTOBUF_FOUND)
find_library(PROTOBUF_LIBRARY protobuf)


# dependency: json
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

##########################################################
# testing
##########################################################
enable_testing()
include(CTest)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)


##########################################################
# common datatypes
##########################################################

set(MESSAGE_PROTOS_COMMON
    datatypes/ActionResult.proto
    datatypes/Ball.proto
    datatypes/Logging.proto
    datatypes/Obstacle.proto
    datatypes/Player.proto
    datatypes/Pose.proto
    datatypes/PosVel.proto
    datatypes/WorldState.proto
)

include_directories(datatypes)
add_library(MRA-datatypes)
target_include_directories(MRA-datatypes PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(MRA-datatypes protobuf)

protobuf_generate(TARGET MRA-datatypes PROTOS ${MESSAGE_PROTOS_COMMON} LANGUAGE cpp IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

# mark the protobuf files as generated source to ensure generation is finished by before building sources.
foreach (protobuf_file ${MESSAGE_PROTOS_COMMON})
    get_filename_component(proto_name ${protobuf_file} NAME_WE)
    get_filename_component(proto_path ${MRA_SOURCE_DIR}/datatypes ABSOLUTE)
    set_source_files_properties(
        "${proto_path}/${proto_name}.pb.cc"
        "${proto_path}/${proto_name}.pb.h"
        PROPERTIES GENERATED TRUE)
endforeach()

# create dummy file for creating libraries with only ohter libraries
file(TOUCH ${MRA_BINARY_DIR}/dummy.cpp)


##########################################################
# subdirectories
##########################################################
#TODO use datatypes/CMakeLists.txt:
# add_subdirectory(datatypes)
add_subdirectory(base)
add_subdirectory(components)
add_subdirectory(libraries)



##########################################################
# create libraries with input from subdirectories
##########################################################
# TODO use datatypes/CMakeLists.txt
#add_library(MRA-datatypes ${MRA_BINARY_DIR}/dummy.cpp)
#target_link_libraries(MRA-datatypes
#    MRA-libraries-dir)
#target_link_directories(MRA-datatypes PUBLIC
#    ${MRA_BINARY_DIR}/datatypes
#)

add_library(MRA-components ${MRA_BINARY_DIR}/dummy.cpp)
target_link_libraries(MRA-components MRA-components-dir)
target_link_directories(MRA-components PUBLIC ${MRA_BINARY_DIR}/components)

add_library(MRA-libraries ${MRA_BINARY_DIR}/dummy.cpp)
target_link_libraries(MRA-libraries MRA-libraries-dir)
target_link_directories(MRA-libraries PUBLIC ${MRA_BINARY_DIR}/libraries)



