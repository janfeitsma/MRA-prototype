#ifndef _MRA_LIBRARIES_MRA_LOGGER_HPP
#define _MRA_LIBRARIES_MRA_LOGGER_HPP

#include <memory>
#include <string>
#include "spdlog/spdlog.h"  // spdlog API: https://github.com/gabime/spdlog


namespace MRA::Logging
{

class MraLogger
{
public:
	static std::shared_ptr<MraLogger> getInstance();

	void logStart(double timestamp, int counter,
			const  std::string& r_input_str,
			const std::string& r_params_str,
			const std::string& r_state_str);

	void logEnd(int counter, int *p_err, double duration, const std::string& r_output_str, const std::string& r_state_str);

	MraLogger(const MraLogger& obj) = delete;

	void setup();

private:
	MraLogger();

	bool m_active = false;
	std::shared_ptr<spdlog::logger> m_spdlog_logger;

}; // class MraLogger

} // namespace MRA::Logging

#endif // #ifndef _MRA_LIBRARIES_LOGGING_HPP
