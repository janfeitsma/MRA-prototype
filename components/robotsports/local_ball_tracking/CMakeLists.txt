##########################################################
# component interfaces
##########################################################

set(MESSAGE_PROTOS_COMPONENTS
    interface/Input.proto
    interface/State.proto
    interface/Params.proto
    interface/Output.proto
    interface/Local.proto    
)

add_library(MRA-components-robotsports-local-ball-tracking-proto)
target_include_directories(MRA-components-robotsports-local-ball-tracking-proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/interface ${MRA_SOURCE_DIR})
target_link_libraries(MRA-components-robotsports-local-ball-tracking-proto MRA-datatypes protobuf)

protobuf_generate(TARGET MRA-components-robotsports-local-ball-tracking-proto PROTOS ${MESSAGE_PROTOS_COMPONENTS} LANGUAGE cpp IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${MRA_SOURCE_DIR})


##########################################################
# component implementations
##########################################################

add_library(MRA-components-robotsports-local-ball-tracking
	tick.cpp
	seq_clustering_best_uid.cpp
	seq_clustering_ball_model.cpp
	seq_clustering_ball_model_log.cpp
	
)
target_include_directories(MRA-components-robotsports-local-ball-tracking PUBLIC 
	${MRA_SOURCE_DIR}/libraries/logging 
	${CMAKE_SOURCE_DIR}/base
	${CMAKE_CURRENT_BINARY_DIR}/interface	)
target_link_libraries(MRA-components-robotsports-local-ball-tracking MRA-libraries MRA-components-robotsports-local-ball-tracking-proto MRA-base nlohmann_json::nlohmann_json)


add_executable(
	robotsports_local_ball_tracking_test
	test.cpp
)
target_include_directories(robotsports_local_ball_tracking_test PRIVATE ${MRA_SOURCE_DIR}/base ${MRA_SOURCE_DIR}/libraries/logging) 
target_link_libraries(
	robotsports_local_ball_tracking_test
	MRA-components MRA-libraries nlohmann_json::nlohmann_json rt GTest::gtest_main gmock
)


gtest_discover_tests(
	robotsports_local_ball_tracking_test
)

