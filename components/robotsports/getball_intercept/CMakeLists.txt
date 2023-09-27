##########################################################
# component interfaces
##########################################################

set(MESSAGE_PROTOS_COMPONENTS
    interface/Input.proto
    # UNAVAILABLE: interface/State.proto
    interface/Params.proto
    interface/Output.proto
    # UNAVAILABLE: interface/Local.proto    
)

add_library(MRA-components-robotsports-getball-intercept-proto)
target_include_directories(MRA-components-robotsports-getball-intercept-proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/interface ${MRA_SOURCE_DIR})
target_link_libraries(MRA-components-robotsports-getball-intercept-proto MRA-datatypes protobuf)

protobuf_generate(TARGET MRA-components-robotsports-getball-intercept-proto PROTOS ${MESSAGE_PROTOS_COMPONENTS} LANGUAGE cpp IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${MRA_SOURCE_DIR})


##########################################################
# component implementations
##########################################################

add_library(MRA-components-robotsports-getball-intercept
	tick.cpp
)
target_include_directories(MRA-components-robotsports-getball-intercept PUBLIC 
	${MRA_SOURCE_DIR}/libraries/logging 
	${CMAKE_SOURCE_DIR}/base
	${CMAKE_CURRENT_BINARY_DIR}/interface	)
target_link_libraries(MRA-components-robotsports-getball-intercept MRA-libraries MRA-components-robotsports-getball-intercept-proto MRA-base nlohmann_json::nlohmann_json)


add_executable(
	robotsports_getball_intercept_test
	test.cpp
)
target_include_directories(robotsports_getball_intercept_test PRIVATE ${MRA_SOURCE_DIR}/base ${MRA_SOURCE_DIR}/libraries/logging) 
target_link_libraries(
	robotsports_getball_intercept_test
	MRA-components MRA-libraries nlohmann_json::nlohmann_json rt GTest::gtest_main gmock
)


gtest_discover_tests(
	robotsports_getball_intercept_test
	WORKING_DIRECTORY ${MRA_SOURCE_DIR}
	PROPERTIES ENVIRONMENT "MRA_LOGGER_CONTEXT=unittest"
)

