cmake_minimum_required(VERSION 3.5.1)

project(VelocityControl)

find_package(Boost COMPONENTS system thread REQUIRED QUIET)

# support switching to debug via -DDEBUG_PACKAGES command line option
apply_debug_if_in_debug_packages("${CMAKE_BUILD_TYPE}")

include_directories("include")


# Reflexxes Type II Motion Library
include_directories(external/ReflexxesTypeII/include)
link_directories(external/ReflexxesTypeII/lib)

add_library(VelocityControl src/VelocityControl.cpp
                            src/VelocityControlData.cpp
                            src/facilities/vcgeometry.cpp
                            src/controllers/StopVelocitySetpointController.cpp
                            src/controllers/LinearVelocitySetpointController.cpp
                            src/controllers/PIDVelocitySetpointController.cpp
                            src/controllers/SPGVelocitySetpointController.cpp
                            src/algorithms/CalculateDeltas.cpp
                            src/algorithms/SelectVelocityController.cpp
                            src/algorithms/CalculateVelocity.cpp
                            src/algorithms/CalculateAccelerating.cpp
                            src/algorithms/Deadzone.cpp
                            src/algorithms/ApplyLimits.cpp
                            src/algorithms/ApplyTokyoDrift.cpp
                            src/algorithms/ShiftBallOffset.cpp
                            src/algorithms/UnShiftBallOffset.cpp)

target_link_libraries(VelocityControl ReflexxesTypeII ${Boost_LIBRARIES})

