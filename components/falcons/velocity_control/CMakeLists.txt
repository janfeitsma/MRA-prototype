# this file was produced by MRA-codegen.py from template_CMakeLists.txt
# it should NOT be modified by user

##########################################################
# component interfaces
##########################################################

set(MESSAGE_PROTOS_COMPONENTS
    interface/Input.proto
    interface/Params.proto
    interface/State.proto
    interface/Output.proto
    interface/Local.proto
)

add_library(MRA-components-falcons-velocity-control-proto)
target_include_directories(MRA-components-falcons-velocity-control-proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/interface ${MRA_SOURCE_DIR})
target_link_libraries(MRA-components-falcons-velocity-control-proto MRA-datatypes protobuf)

protobuf_generate(TARGET MRA-components-falcons-velocity-control-proto PROTOS ${MESSAGE_PROTOS_COMPONENTS} LANGUAGE cpp IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR} ${MRA_SOURCE_DIR})


##########################################################
# component implementations
##########################################################

add_library(MRA-components-falcons-velocity-control
    tick.cpp
    internal/src/VelocityControl.cpp
    internal/src/controllers/LinearVelocitySetpointController.cpp
    internal/src/controllers/SPGVelocitySetpointController.cpp
    internal/src/algorithms/CheckPrepareInputs.cpp
    internal/src/algorithms/CheckStop.cpp
    internal/src/algorithms/ConfigureLimits.cpp
    internal/src/algorithms/Deadzone.cpp
    internal/src/algorithms/SelectVelocityController.cpp
    internal/src/algorithms/ShiftBallOffset.cpp
    internal/src/algorithms/CalculateVelocity.cpp
    internal/src/algorithms/SetOutputsPrepareNext.cpp
)
target_include_directories(MRA-components-falcons-velocity-control PUBLIC
    ${MRA_SOURCE_DIR}/libraries/logging
    ${CMAKE_SOURCE_DIR}/base
    ${CMAKE_CURRENT_BINARY_DIR}/interface
    ${CMAKE_CURRENT_SOURCE_DIR}/internal/include
)
target_link_libraries(MRA-components-falcons-velocity-control MRA-libraries MRA-components-falcons-velocity-control-proto MRA-base nlohmann_json::nlohmann_json)


add_executable(
    falcons_velocity_control_test
    test.cpp
)
target_include_directories(falcons_velocity_control_test PRIVATE ${MRA_SOURCE_DIR}/base ${MRA_SOURCE_DIR}/libraries/logging)
target_link_libraries(
    falcons_velocity_control_test
    MRA-components MRA-libraries nlohmann_json::nlohmann_json rt GTest::gtest_main gmock
)


gtest_discover_tests(
    falcons_velocity_control_test
    WORKING_DIRECTORY ${MRA_SOURCE_DIR}
    PROPERTIES ENVIRONMENT "MRA_LOGGER_CONTEXT=unittest"
)

