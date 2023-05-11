/*
 * VelocityControlExceptions.hpp
 *
 *  Created on: May 2023
 *      Author: Jan Feitsma
 */

#ifndef VELOCITYCONTROL_EXCEPTIONS_HPP_
#define VELOCITYCONTROL_EXCEPTIONS_HPP_

#include "exception_base.hpp"


namespace VelocityControlExceptions
{

DECLARE_CUSTOM_EXCEPTION(RobotInactive);
DECLARE_CUSTOM_EXCEPTION(InvalidInput);
DECLARE_CUSTOM_EXCEPTION(IncompleteInput);
DECLARE_CUSTOM_EXCEPTION(IncompleteConfiguration);
DECLARE_CUSTOM_EXCEPTION(UnsupportedDimension);

}

#endif // #ifndef VELOCITYCONTROL_EXCEPTIONS_HPP_

