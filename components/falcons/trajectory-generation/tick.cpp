// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsTrajectoryGeneration.hpp"

// dependent (generated) component headers:
#include "FalconsVelocityControl.hpp"

using namespace MRA;

// custom includes, if any
#include "geometry.hpp"


int FalconsTrajectoryGeneration::FalconsTrajectoryGeneration::tick
(
    google::protobuf::Timestamp timestamp,   // absolute timestamp
    InputType  const           &input,       // input data, type generated from Input.proto
    ParamsType const           &params,      // configuration parameters, type generated from Params.proto
    StateType                  &state,       // state data, type generated from State.proto
    OutputType                 &output,      // output data, type generated from Output.proto
    LocalType                  &local        // local/diagnostics data, type generated from Local.proto
)
{
    int error_value = 0;

    // user implementation goes here

    // return immediately when inactive
    if (!input.worldstate().robot().active()) return 0;

    // setup
    auto vcModel = MRA::FalconsVelocityControl::FalconsVelocityControl();
    auto vcInput = MRA::FalconsVelocityControl::Input();
    vcInput.mutable_worldstate()->mutable_robot()->mutable_position()->set_x(0.0);
    vcInput.mutable_worldstate()->mutable_robot()->mutable_velocity()->set_x(0.0);
    vcInput.mutable_worldstate()->MergeFrom(input.worldstate());
    vcInput.mutable_setpoint()->MergeFrom(input.setpoint());
    vcInput.set_motionprofile(input.motionprofile());
    auto vcState = MRA::FalconsVelocityControl::State();
    auto vcOutput = MRA::FalconsVelocityControl::Output();
    auto vcLocal = MRA::FalconsVelocityControl::Local();

    // configure VelocityControl, overruling its default configuration
    //    auto vcParams = vcModel.defaultParams();
    //    vcParams.MergeFrom(params.vcparams()); // with vcParams a nested message
    // these 2 lines do not work because zeros are ignored, even when explicitly set ...
    // it appears protobuf v3 made a design mistake by entangling default values with presence tracking?
    // -> we made a new template which internally uses json.update
    auto vcParams = vcModel.defaultParams();
    try
    {
        MRA::merge_json_into_proto(params.vcparamsjsonstr(), vcParams);
    }
    catch (...)
    {
        std::cerr << "failed to reconfigure VelocityControl with json string (" << params.vcparamsjsonstr() << ")" << std::endl;
        return 254;
    }

    // configure the rest
    double dt = vcParams.dt();
    int maxTicks = params.maxticks();

    // iterate
    // NOTE: vcInput.worldstate will be (ab)used, update each iteration as simulated state
    google::protobuf::Timestamp sim_timestamp = timestamp;
    int tick_counter = 0;
    while (true)
    {
        tick_counter++;
        sim_timestamp += google::protobuf::util::TimeUtil::NanosecondsToDuration(dt * 1e9);

        // call model tick
        int error_value = vcModel.tick(sim_timestamp, vcInput, vcParams, vcState, vcOutput, vcLocal);

        // use MRA geometry library for display and simulated worldstate update
        MRA::Geometry::Position pos_fcs(vcInput.worldstate().robot().position());
        MRA::Geometry::Velocity vel_fcs(vcInput.worldstate().robot().velocity());
        MRA::Geometry::Velocity vel_rcs(vcOutput.velocity());

        // check for errors
        if (error_value)
        {
            std::cerr << "tick " << tick_counter << " failed with error " << error_value << std::endl;
            return error_value;
        }

        // check for non-convergence
        if (tick_counter > maxTicks)
        {
            std::cerr << "tick limit exceeded (" << maxTicks << ")" << std::endl;
            return 255;
        }

        // check for convergence - stop criterion is that output velocity is zero
        // (alternative: actionresult PASSED/FAILED)
        if (vel_rcs.is_zero())
        {
            break;
        }

        // simulate robot movement: update WorldState input for next tick, by applying velocity output
        vel_fcs = vel_rcs.transformRcsToFcs(pos_fcs);
        pos_fcs += vel_fcs * dt;
        auto robot = vcInput.mutable_worldstate()->mutable_robot();
        robot->mutable_position()->set_x(pos_fcs.x);
        robot->mutable_position()->set_y(pos_fcs.y);
        robot->mutable_position()->set_rz(pos_fcs.rz);
        robot->mutable_velocity()->set_x(vel_fcs.x);
        robot->mutable_velocity()->set_y(vel_fcs.y);
        robot->mutable_velocity()->set_rz(vel_fcs.rz);

        // store the sample in output
        MRA::FalconsTrajectoryGeneration::Sample *sample = output.add_samples();
        sample->set_t(dt * tick_counter);
        *sample->mutable_position() = robot->position();
        *sample->mutable_velocity() = robot->velocity();
    }

    // finish
    output.set_duration(dt * tick_counter);
    output.set_numticks(tick_counter);
    *output.mutable_final()->mutable_position() = vcInput.worldstate().robot().position();
    *output.mutable_final()->mutable_velocity() = vcInput.worldstate().robot().velocity();

    return error_value;
}

