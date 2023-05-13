// this file was produced by MRA-codegen.py from template_tick.cpp
// with the intent of letting user add the implementation here

// generated component header:
#include "FalconsTrajectoryGeneration.hpp"

// dependent (generated) component headers:
#include "FalconsVelocityControl.hpp"

using namespace MRA;

// custom includes, if any
#include "geometry.hpp"
//#include <iostream>
//#include <string>


int FalconsTrajectoryGeneration::FalconsTrajectoryGeneration::tick
(
    double            timestamp,   // simulation timestamp, seconds since start of simulation
    InputType  const &input,       // input data, type generated from Input.proto
    ParamsType const &params,      // configuration parameters, type generated from Params.proto
    StateType        &state,       // state data, type generated from State.proto
    OutputType       &output,      // output data, type generated from Output.proto
    LocalType        &local        // local/diagnostics data, type generated from Local.proto
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
    auto vcParams = vcModel.defaultParams();

    // configure
    // (these could become the params of FalconsTrajectoryGeneration?)
    double dt = vcParams.dt();
    int MAX_TICKS = 1000;
    bool print_table = false;
    bool print_errors = true;

    // table row header
    std::string header_row_1 = "                  |                   input.worldstate (FCS)                  |     output.velocity (RCS)  ";
    std::string header_row_2 = "   iter         t |   pos.x     pos.y    pos.rz     vel.x     vel.y    vel.rz |   vel.x     vel.y    vel.rz";
    std::string header_row_sep = "===========================================================================================================";
    if (print_table)
    {
        std::cout << header_row_1 << std::endl;
        std::cout << header_row_2 << std::endl;
        std::cout << header_row_sep << std::endl;
    }

    // iterate
    double sim_timestamp = 0.0;
    int tick_counter = 0;
    while (true)
    {
        tick_counter++;
        sim_timestamp += dt;

        // call model tick
        int error_value = vcModel.tick(sim_timestamp, vcInput, vcParams, vcState, vcOutput, vcLocal);

        // use MRA geometry library for display and simulated worldstate update
        MRA::Geometry::Position pos_fcs(vcInput.worldstate().robot().position());
        MRA::Geometry::Velocity vel_fcs(vcInput.worldstate().robot().velocity());
        MRA::Geometry::Velocity vel_rcs(vcOutput.velocity());

        // check for errors
        if (error_value)
        {
            if (print_errors)
            {
                std::cerr << "tick " << tick_counter << " failed with error " << error_value << std::endl;
            }
            return error_value;
        }

        // check for non-convergence
        if (tick_counter > MAX_TICKS)
        {
            if (print_errors)
            {
                std::cerr << "tick limit exceeded (" << MAX_TICKS << ")" << std::endl;
            }
            return 255;
        }

        // display a row in the table
        if (print_table)
        {
            printf("%7d %9.4f", tick_counter, sim_timestamp);
            printf("%s", pos_fcs.xyrz_str().c_str());
            printf("%s", vel_fcs.xyrz_str().c_str());
            printf("%s", vel_rcs.xyrz_str().c_str());
            printf("\n");
        }

        // check for convergence - stop criterion is that output velocity is zero
        // (alternative: actionresult PASSED/FAILED)
        if (vcOutput.ByteSizeLong() == 0) // a bit magic, but: bytesize 0 means all elements are 0
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
    }

    // table row footer
    if (tick_counter > 20 && print_table)
    {
        std::cout << header_row_sep << std::endl;
        std::cout << header_row_2 << std::endl;
        std::cout << header_row_1 << std::endl;
    }

    // set output
    output.set_duration(sim_timestamp);
    *output.mutable_final()->mutable_position() = vcInput.worldstate().robot().position();
    *output.mutable_final()->mutable_velocity() = vcInput.worldstate().robot().velocity();

    return error_value;
}

