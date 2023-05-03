/*
 * vcConfigRTDBAdapter.hpp
 *
 *  Created on: Nov, 2020
 *      Author: Erik Kouters
 */

#ifndef VCCONFIGRTDBADAPTER_VELOCITYCONTROL_HPP_
#define VCCONFIGRTDBADAPTER_VELOCITYCONTROL_HPP_

#include "FalconsRTDB.hpp"
#include "ConfigRTDBAdapter.hpp"


class vcConfigRTDBAdapter : public ConfigRTDBAdapter<ConfigVelocityControl>
{
public:
    vcConfigRTDBAdapter(std::string const &configKey);
    ~vcConfigRTDBAdapter();

    void loadYAML(std::string const &yamlFile);

private:

};

#endif

