cmake_minimum_required(VERSION 3.5.1)

##########################################################
# general configuration
##########################################################

project(MRA LANGUAGES CXX)

# use C++17 standard, or suffer errors re. using std::filesystem
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror -Wno-address -Wno-reorder")
 # to prevent compiler errors added the not preferred flags: -Wno-address -Wno-reorder

# dependency: ProtoBuf
find_package(Protobuf REQUIRED)
find_library(PROTOBUF_LIBRARY protobuf)

# check if spdlog is installed
# install on Ubuntu: sudo apt-get install libspdlog-dev
find_package(spdlog REQUIRED)


# dependency: json
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)


##########################################################
# common datatypes
##########################################################

set(MESSAGE_PROTOS_COMMON
    datatypes/ActionResult.proto
    datatypes/Ball.proto
    datatypes/Logging.proto
    datatypes/Obstacle.proto
    datatypes/Player.proto
    datatypes/Pose.proto
    datatypes/PosVel.proto
    datatypes/WorldState.proto
)

include_directories(datatypes)
add_library(MRA-datatypes)
target_include_directories(MRA-datatypes PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(MRA-datatypes protobuf)

protobuf_generate(TARGET MRA-datatypes PROTOS ${MESSAGE_PROTOS_COMMON} LANGUAGE cpp IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR})



##########################################################
# common libraries
##########################################################

add_library(MRA-libraries
    libraries/geometry/angles.cpp
    libraries/geometry/pose.cpp
    libraries/geometry/position.cpp
    libraries/geometry/velocity.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLStep2PhaseSynchronization.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLStep2Profiles.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLStep2WithoutSynchronization.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLStep3.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLVelocityCalculatePositionalExtrems.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLVelocity.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLVelocityFallBackStrategy.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLVelocityIsPhaseSynchronizationPossible.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLVelocityMethods.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLVelocitySetupPhaseSyncSelectionVector.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/ReflexxesAPI.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLCalculatePositionalExtrems.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLDecisions.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1A.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1B.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1C.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLDecisionTree2.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLFallBackStrategy.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLIsPhaseSynchronizationPossible.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLPolynomial.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLPosition.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLQuicksort.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLSetupModifiedSelectionVector.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLStep1.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLStep1IntermediateProfiles.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLStep1Profiles.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLStep2.cpp
    libraries/reflexxes/RMLTypeII/src/TypeIIRML/TypeIIRMLStep2IntermediateProfiles.cpp
)
target_include_directories(MRA-datatypes PUBLIC libraries/geometry libraries/reflexxes/RMLTypeII/include .)
target_link_libraries(MRA-libraries MRA-datatypes)


##########################################################
# base headers / utilities
##########################################################

include_directories(base)

add_library(MRA-base
    base/json_convert.cpp
)
target_link_libraries(MRA-base nlohmann_json::nlohmann_json)

##########################################################
# component interfaces
##########################################################

set(MESSAGE_PROTOS_COMPONENTS
    components/falcons/getball_fetch/interface/Input.proto
    components/falcons/getball_fetch/interface/Params.proto
    components/falcons/getball_fetch/interface/Output.proto
    components/falcons/velocity_control/interface/Input.proto
    components/falcons/velocity_control/interface/Local.proto
    components/falcons/velocity_control/interface/Params.proto
    components/falcons/velocity_control/interface/Output.proto
    components/falcons/velocity_control/interface/State.proto
    components/robotsports/getball_intercept/interface/Input.proto
    components/robotsports/getball_intercept/interface/Params.proto
    components/robotsports/getball_intercept/interface/Output.proto
    components/robotsports/proof_is_alive/interface/Input.proto
    components/robotsports/proof_is_alive/interface/Local.proto
    components/robotsports/proof_is_alive/interface/Params.proto
    components/robotsports/proof_is_alive/interface/Output.proto
    components/robotsports/proof_is_alive/interface/State.proto
)

add_library(MRA-components-proto)
target_include_directories(MRA-components-proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(MRA-components-proto MRA-datatypes protobuf)

protobuf_generate(TARGET MRA-components-proto PROTOS ${MESSAGE_PROTOS_COMPONENTS} LANGUAGE cpp IMPORT_DIRS ${CMAKE_CURRENT_SOURCE_DIR})


##########################################################
# component implementations
##########################################################

add_library(MRA-components
    components/falcons/getball_fetch/tick.cpp
    components/falcons/velocity_control/tick.cpp
    components/falcons/velocity_control/internal/src/VelocityControl.cpp
    components/falcons/velocity_control/internal/src/controllers/LinearVelocitySetpointController.cpp
    components/falcons/velocity_control/internal/src/controllers/SPGVelocitySetpointController.cpp
    components/falcons/velocity_control/internal/src/algorithms/CheckPrepareInputs.cpp
    components/falcons/velocity_control/internal/src/algorithms/CheckStop.cpp
    components/falcons/velocity_control/internal/src/algorithms/ConfigureLimits.cpp
    components/falcons/velocity_control/internal/src/algorithms/Deadzone.cpp
    components/falcons/velocity_control/internal/src/algorithms/SelectVelocityController.cpp
    components/falcons/velocity_control/internal/src/algorithms/ShiftBallOffset.cpp
    components/falcons/velocity_control/internal/src/algorithms/CalculateVelocity.cpp
    components/falcons/velocity_control/internal/src/algorithms/SetOutputsPrepareNext.cpp

    components/robotsports/getball_intercept/tick.cpp
    components/robotsports/proof_is_alive/tick.cpp
)
target_include_directories(MRA-components PUBLIC components/falcons/velocity_control/internal/include 
                                                 libraries/logging)
target_link_libraries(MRA-components MRA-libraries MRA-components-proto MRA-base nlohmann_json::nlohmann_json)
