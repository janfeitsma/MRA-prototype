#ifndef _MRA_LIBRARIES_LOGGING_CONTEXT_HPP
#define _MRA_LIBRARIES_LOGGING_CONTEXT_HPP

#include <string>

namespace MRA::Logging
{

std::string getComponentName();
void setComponentName(std::string const &c);

}

#endif // #ifndef _MRA_LIBRARIES_LOGGING_CONTEXT_HPP

