#ifndef _MRA_LIBRARIES_LOGGING_CONTROL_HPP
#define _MRA_LIBRARIES_LOGGING_CONTROL_HPP


// define free functions to control logging and query logger status

// the configuration struct is defined in protobuf format
#include "datatypes/Logging.pb.h"


namespace MRA::Logging
{

// get the logging folder, may create if needed
std::string logFolder();

// configure
MRA::Datatypes::LogControl getConfiguration(); // get entire configuration
MRA::Datatypes::LogSpec getConfiguration(std::string const &component); // get the configuration that applies to given component
void setConfiguration(MRA::Datatypes::LogControl const &config);
void setConfiguration(std::string const &component, MRA::Datatypes::LogSpec const &config);

} // namespace MRA::Logging

#endif // #ifndef _MRA_LIBRARIES_LOGGING_CONTROL_HPP

