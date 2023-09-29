##########################################################
# common datatypes
##########################################################

set(MESSAGE_PROTOS_COMMON
    ActionResult.proto
    Ball.proto
    Logging.proto
    Obstacle.proto
    Player.proto
    Pose.proto
    PosVel.proto
    WorldState.proto
)
message("CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
message("MRA_SOURCE_DIR=${MRA_SOURCE_DIR}")
message("MRA_SOURCE_DIR=${MRA_BINARY_DIR}")
message("CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")
message("CMAKE_CURRENT_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${MRA_BINARY_DIR} ${MRA_SOURCE_DIR})
add_library(MRA-datatypes-dir)
target_include_directories(MRA-datatypes-dir PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${MRA_BINARY_DIR} ${MRA_SOURCE_DIR})
target_link_libraries(MRA-datatypes-dir protobuf)
set(PROTO_IMPORT_DIRS "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" "${MRA_BINARY_DIR}" "${MRA_SOURCE_DIR}" )
protobuf_generate(TARGET MRA-datatypes-dir PROTOS ${MESSAGE_PROTOS_COMMON} LANGUAGE cpp IMPORT_DIRS ${PROTO_IMPORT_DIRS})

# mark the protobuf files as generated source to ensure generation is finished by before building sources.
foreach (protobuf_file ${MESSAGE_PROTOS_COMMON})
    get_filename_component(proto_name ${protobuf_file} NAME_WE)
    get_filename_component(proto_path ${MRA_SOURCE_DIR}/datatypes ABSOLUTE)
    set_source_files_properties(
        "${proto_path}/${proto_name}.pb.cc"
        "${proto_path}/${proto_name}.pb.h"
        PROPERTIES GENERATED TRUE)
endforeach()

