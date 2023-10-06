# this file was produced by MRA-codegen.py from template_CMakeLists.txt
# it should NOT be modified by user

add_library(MRA-components-robotsports-getball-intercept
    tick.cpp
)
target_include_directories(MRA-components-robotsports-getball-intercept PUBLIC
    ${MRA_SOURCE_DIR}/libraries/logging
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/base
    ${CMAKE_CURRENT_BINARY_DIR}/interface)
target_link_libraries(MRA-components-robotsports-getball-intercept MRA-libraries MRA-components-proto MRA-base nlohmann_json::nlohmann_json)


add_executable(
    robotsports_getball_intercept_test
    test.cpp
)
target_include_directories(robotsports_getball_intercept_test PRIVATE ${MRA_SOURCE_DIR}/base ${MRA_SOURCE_DIR}/libraries/logging)
target_link_libraries(
    robotsports_getball_intercept_test
    MRA-components-robotsports-getball-intercept
    MRA-libraries MRA-components-proto nlohmann_json::nlohmann_json rt GTest::gtest_main gmock
)


gtest_discover_tests(
    robotsports_getball_intercept_test
    WORKING_DIRECTORY ${MRA_SOURCE_DIR}
    PROPERTIES ENVIRONMENT "MRA_LOGGER_CONTEXT=unittest"
)

