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


include_directories(${MRA_SOURCE_DIR}/datatypes)
add_library(MRA-datatypes)
target_include_directories(MRA-datatypes PUBLIC 
		${CMAKE_CURRENT_BINARY_DIR} 
		${CMAKE_CURRENT_SOURCE_DIR} 
		${MRA_SOURCE_DIR}/datatypes
		${CMAKE_BINARY_DIR} )
target_link_libraries(MRA-datatypes protobuf)

protobuf_generate(TARGET MRA-datatypes PROTOS ${MESSAGE_PROTOS_COMMON} LANGUAGE cpp 
		IMPORT_DIRS 
			${CMAKE_CURRENT_BINARY_DIR} 
			${CMAKE_CURRENT_SOURCE_DIR} 
			${MRA_SOURCE_DIR} 
			${CMAKE_BINARY_DIR})
