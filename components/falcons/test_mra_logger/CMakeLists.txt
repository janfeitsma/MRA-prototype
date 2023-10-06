# this file was produced by MRA-codegen.py from template_CMakeLists.txt
# it should NOT be modified by user

add_library(MRA-components-falcons-test-mra-logger
    tick.cpp
    src/demofunctions.cpp
    src/test_helpers.cpp
)
target_include_directories(MRA-components-falcons-test-mra-logger PUBLIC
    ${MRA_SOURCE_DIR}/libraries/logging
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/base
    ${CMAKE_CURRENT_BINARY_DIR}/interface)
target_link_libraries(MRA-components-falcons-test-mra-logger MRA-libraries MRA-components-proto MRA-base nlohmann_json::nlohmann_json)


add_executable(
    falcons_test_mra_logger_test
    test.cpp
)
target_include_directories(falcons_test_mra_logger_test PRIVATE ${MRA_SOURCE_DIR}/base ${MRA_SOURCE_DIR}/libraries/logging)
target_link_libraries(
    falcons_test_mra_logger_test
    MRA-components-falcons-test-mra-logger
    MRA-libraries MRA-components-proto nlohmann_json::nlohmann_json rt GTest::gtest_main gmock
)


gtest_discover_tests(
    falcons_test_mra_logger_test
    WORKING_DIRECTORY ${MRA_SOURCE_DIR}
    PROPERTIES ENVIRONMENT "MRA_LOGGER_CONTEXT=unittest"
)

