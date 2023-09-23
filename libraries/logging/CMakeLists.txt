##########################################################
# component implementations
##########################################################

add_library(MRA-libraries-logging
    backend.cpp
    control.cpp
)

message("MRA_BINARY_DIR = ${MRA_BINARY_DIR}")
target_include_directories(MRA-libraries-logging PUBLIC 
		${MRA_BINARY_DIR}
		${CMAKE_SOURCE_DIR}/base
)
target_link_libraries(MRA-libraries-logging MRA-base nlohmann_json::nlohmann_json rt)