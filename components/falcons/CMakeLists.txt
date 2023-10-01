# add_subdirectory(getball)
add_subdirectory(getball_fetch)
add_subdirectory(getball_intercept)
add_subdirectory(test_mra_logger)
# TODO: add_subdirectory(trajectory_generation)
add_subdirectory(velocity_control)

add_library(MRA-components-falcons ${MRA_BINARY_DIR}/dummy.cpp)
target_link_libraries(MRA-components-falcons
					  # TODO: MRA-falcons-getball
					  MRA-components-falcons-getball-fetch
					  MRA-components-falcons-getball-intercept
					  MRA-components-falcons-test-mra-logger
					  # TODO: MRA-falcons-trajectory-generation
					  MRA-components-falcons-velocity-control
)

target_link_directories(MRA-components-falcons PUBLIC
	${MRA_BINARY_DIR}/components/falcons/getball_fetch
	${MRA_BINARY_DIR}/components/falcons/getball_intercept
	${MRA_BINARY_DIR}/components/falcons/test_mra_logger
	${MRA_BINARY_DIR}/components/falcons/velocity_control
)
