cmake_minimum_required(VERSION 3.5.1)

project(MRA-datatypes)

find_package(Protobuf REQUIRED)
find_library(PROTOBUF_LIBRARY protobuf)

set(MESSAGE_PROTOS
    ActionResult.proto
    Pose.proto
    Ball.proto
    Obstacle.proto
    Player.proto
    Target.proto
    WorldState.proto
)

add_library(datatypes )
target_include_directories(datatypes PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/..)
target_link_libraries(datatypes protobuf)

# inner .proto imports are relative to MRA root
protobuf_generate(TARGET datatypes PROTOS ${MESSAGE_PROTOS} LANGUAGE cpp IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/..)


