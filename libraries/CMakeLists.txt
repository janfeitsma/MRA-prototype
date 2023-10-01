add_subdirectory(geometry)
add_subdirectory(logging)
add_subdirectory(reflexxes)

add_library(MRA-libraries-dir ${MRA_BINARY_DIR}/dummy.cpp)

target_link_libraries(MRA-libraries-dir
				   MRA-libraries-geometry
   				   MRA-libraries-logging
   				   MRA-libraries-reflexxes
)
target_link_directories(MRA-libraries-dir PUBLIC
	${MRA_BINARY_DIR}/libraries/geometry
	${MRA_BINARY_DIR}/libraries/logging
	${MRA_BINARY_DIR}/libraries/reflexxes
)

