add_subdirectory(falcons)
add_subdirectory(robotsports)

add_library(MRA-components-dir ${MRA_BINARY_DIR}/dummy.cpp)
target_link_libraries(MRA-components-dir
    MRA-components-falcons
    MRA-components-robotsports
)

target_link_directories(MRA-components-dir PUBLIC
    ${MRA_BINARY_DIR}/components/falcons
    ${MRA_BINARY_DIR}/components/robotsports
)

