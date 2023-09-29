add_subdirectory(getball_fetch)
add_subdirectory(getball_intercept)
add_subdirectory(proof_is_alive)

add_library(MRA-components-robotsports ${MRA_BINARY_DIR}/dummy.cpp)
target_link_libraries(MRA-components-robotsports
    MRA-components-robotsports-getball-fetch
    MRA-components-robotsports-getball-intercept
    MRA-components-robotsports-proof-is-alive)

target_link_directories(MRA-components-robotsports PUBLIC
    ${MRA_BINARY_DIR}/components/robotsports/getball_fetch
    ${MRA_BINARY_DIR}/components/robotsports/getball_intercept
    ${MRA_BINARY_DIR}/components/robotsports/proof_is_alive
)

