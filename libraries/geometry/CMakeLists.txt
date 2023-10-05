##########################################################
# component implementations
##########################################################

add_library(MRA-libraries-geometry
	angles.cpp
    pose.cpp
    position.cpp
    velocity.cpp
)

target_include_directories(MRA-libraries-geometry PUBLIC
	${MRA_BINARY_DIR}
)
add_dependencies(MRA-libraries-geometry MRA-datatypes)
