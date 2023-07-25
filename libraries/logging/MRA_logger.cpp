#include "MRA_logger.hpp"
#include <memory>

using namespace MRA::Logging;

std::shared_ptr<MraLogger> MraLogger::getInstance() {
	static std::shared_ptr<MraLogger> sp_logger(new MraLogger);
	return sp_logger;
}

