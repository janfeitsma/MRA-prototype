/*
 * ConfigureLimits.cpp
 *
 *  Created on: May 2023
 *      Author: Jan Feitsma
 */

#include "VelocityControlAlgorithms.hpp"
#include "VelocityControlExceptions.hpp"


void ConfigureLimits::execute(VelocityControlData &data)
{
    // check that at least one set of limits is configured
    int num_motionprofiles = data.config.limits().size();
    if (num_motionprofiles == 0)
    {
        throw VelocityControlExceptions::IncompleteConfiguration(__FILE__, __LINE__, "require at least one set of motionprofile limits");
    }

    // on input, user can select which motion profile to use, default 0
    int motionprofile = data.input.motionprofile();
    if (motionprofile < 0 || motionprofile >= num_motionprofiles)
    {
        throw VelocityControlExceptions::InvalidInput(__FILE__, __LINE__, "invalid motionprofile requested: " + std::to_string(motionprofile)
            + " (number of configured motionprofiles: " + std::to_string(num_motionprofiles) + ")");
    }

    // when robot has the ball, and if input motion profile is default (0), 
    // then automatically choose a set of motion profiles called 'withBall' from the configured non-default motion profiles
    std::string expected_withball_name = "withBall"; // TODO: allow more names? this is a little bit magic.
    if (data.input.worldstate().robot().hasball() && motionprofile == 0)
    {
        for (int tmp_motionprofile = 1; tmp_motionprofile < num_motionprofiles; ++tmp_motionprofile)
        {
            if (data.config.limits(tmp_motionprofile).name() == expected_withball_name)
            {
                motionprofile = tmp_motionprofile;
            }
        }
    }

    // fill limits based on default motion profile
    // then, use requested motionprofile to overrule
    // doing this after filling based on the default set (index 0) helps to keep the configuration clean
    data.limits.CopyFrom(data.config.limits(0));
    data.limits.MergeFrom(data.config.limits(motionprofile));
    // TODO: setting some value to zero might not work in protobuf v3 with this code
}

