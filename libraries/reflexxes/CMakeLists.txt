##########################################################
# component implementations
##########################################################

add_library(MRA-libraries-reflexxes
    RMLTypeII/src/TypeIIRML/TypeIIRMLStep2PhaseSynchronization.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLStep2Profiles.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLStep2WithoutSynchronization.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLStep3.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLVelocityCalculatePositionalExtrems.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLVelocity.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLVelocityFallBackStrategy.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLVelocityIsPhaseSynchronizationPossible.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLVelocityMethods.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLVelocitySetupPhaseSyncSelectionVector.cpp
    RMLTypeII/src/TypeIIRML/ReflexxesAPI.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLCalculatePositionalExtrems.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLDecisions.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1A.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1B.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLDecisionTree1C.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLDecisionTree2.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLFallBackStrategy.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLIsPhaseSynchronizationPossible.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLPolynomial.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLPosition.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLQuicksort.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLSetupModifiedSelectionVector.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLStep1.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLStep1IntermediateProfiles.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLStep1Profiles.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLStep2.cpp
    RMLTypeII/src/TypeIIRML/TypeIIRMLStep2IntermediateProfiles.cpp
)

target_include_directories(MRA-libraries-reflexxes PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/RMLTypeII/include
	${MRA_SOURCE_DIR}/libraries/geometry
)
