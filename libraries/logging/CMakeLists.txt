
# some dependencies need to be newer than provided by Ubuntu20, so we use FetchContent for them
include(FetchContent)

# get a newer version of spdlog than Ubuntu20 provides
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.11.0
)
FetchContent_MakeAvailable(spdlog)

add_library(MRA-libraries-logging
    backend.cpp
    control.cpp
    context.cpp
)

target_include_directories(MRA-libraries-logging PUBLIC
		${MRA_BINARY_DIR}
		${CMAKE_SOURCE_DIR}/base
)
target_link_libraries(MRA-libraries-logging MRA-base nlohmann_json::nlohmann_json rt spdlog::spdlog)
