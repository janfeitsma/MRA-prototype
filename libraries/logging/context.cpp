#include "context.hpp"

namespace MRA::Logging
{

static std::string s_componentName;

std::string getComponentName()
{
    return s_componentName;
}

void setComponentName(std::string const &c)
{
    s_componentName = c;
}

} // namespace MRA::Logging

