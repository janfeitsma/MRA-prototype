#ifndef MRA_COMPONENTS_FALCONS_VELOCITYCONTROL_MRA_BRIDGE_HPP_
#define MRA_COMPONENTS_FALCONS_VELOCITYCONTROL_MRA_BRIDGE_HPP_

// map Falcons Position2D, Velocity2D pose to MRA classes and operations (at MRA-libraries)
#include "geometry.hpp"
typedef MRA::Geometry::Position Position2D;
typedef MRA::Geometry::Velocity Velocity2D;
typedef MRA::Geometry::Pose pose;

// VelocityControlData is now basically a collection of protobuf datatypes
#include "components/falcons/velocity_control/interface/Input.pb.h"
#include "components/falcons/velocity_control/interface/Params.pb.h"
#include "components/falcons/velocity_control/interface/Output.pb.h"
#include "components/falcons/velocity_control/interface/State.pb.h"
#include "components/falcons/velocity_control/interface/Local.pb.h"
typedef google::protobuf::Timestamp MRA_timestamp;

typedef MRA::FalconsVelocityControl::Input  MRA_InputType;
typedef MRA::FalconsVelocityControl::Params MRA_ParamsType;
typedef MRA::FalconsVelocityControl::State  MRA_StateType;
typedef MRA::FalconsVelocityControl::Output MRA_OutputType;
typedef MRA::FalconsVelocityControl::Local  MRA_LocalType;

// map configuration types
typedef MRA::FalconsVelocityControl::SpgConfig SpgConfig;

#endif

