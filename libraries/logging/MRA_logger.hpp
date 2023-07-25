#ifndef _MRA_LIBRARIES_MRA_LOGGER_HPP
#define _MRA_LIBRARIES_MRA_LOGGER_HPP

#include <memory>

namespace MRA::Logging
{

class MraLogger
{
public:
	static std::shared_ptr<MraLogger> getInstance();

private:

}; // class MraLogger

} // namespace MRA::Logging

#endif // #ifndef _MRA_LIBRARIES_LOGGING_HPP

