#include "MRA_logger.hpp"
#include <memory>
#include "spdlog/spdlog.h"  // spdlog API: https://github.com/gabime/spdlog
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <stdarg.h>

using namespace MRA::Logging;

std::shared_ptr<MraLogger> MraLogger::getInstance() {
	static std::shared_ptr<MraLogger> sp_logger(new MraLogger);
	return sp_logger;
}

MraLogger::MraLogger() {
	setup();
}


void MraLogger::setup()
{
	m_active = true;
    m_spdlog_logger = spdlog::basic_logger_mt<spdlog::async_factory>("MRA", "/tmp/async_log.txt");
}

void MraLogger::setPreLogText(const std::string& r_pretext) {
	m_pretext = r_pretext;
}


void MraLogger::logStart(double timestamp, int counter,
		const std::string& r_input_str, const std::string& r_params_str, const std::string& r_state_str)
{
	this->log(MraLogger::INFO, "tick %d START", counter);
	this->log(MraLogger::INFO, "timestamp: %6.3f", timestamp);
	this->log(MraLogger::INFO, "input: %s", r_input_str.c_str());
	this->log(MraLogger::INFO, "params: %s", r_params_str.c_str());
	this->log(MraLogger::INFO, "state: %s", r_state_str.c_str());
}


void MraLogger::logEnd(int counter, int *p_err, double duration, const std::string& r_output_str, const std::string& r_state_str)
{
	this->log(MraLogger::INFO, "tick %d END error_value=%d", counter, *p_err);
	this->log(MraLogger::INFO, "duration: %6.3f", duration);
	this->log(MraLogger::INFO, "output: %s", r_output_str.c_str());
	this->log(MraLogger::INFO, "state: %s", r_state_str.c_str());
}

void MraLogger::log(MraLogger::LogLevel loglevel, const char *fmt,...)
{
	if (m_active) {
		const int MAXTEXT = 4096;
		char buffer[MAXTEXT];
		buffer[MAXTEXT-1] = '\0';
		va_list argptr;
		va_start(argptr, fmt);
		vsprintf(buffer, fmt, argptr);
		va_end(argptr);
		if (buffer[MAXTEXT-1])
		{
			buffer[MAXTEXT-1] = '\0';
			m_spdlog_logger->critical("fatal error, string overflow in %s\n", __FUNCTION__);
			m_spdlog_logger->critical("tried to process:\n%s...\n", buffer);
			exit(-1);
		}
		else {
			switch (loglevel) {
			case MraLogger::CRITICAL:
				m_spdlog_logger->critical(m_pretext + buffer);
				break;
			case MraLogger::ERROR:
				m_spdlog_logger->error(m_pretext + buffer);
				break;
			case MraLogger::WARNING:
				m_spdlog_logger->warn(m_pretext + buffer);
				break;
			case MraLogger::INFO:
				m_spdlog_logger->info(m_pretext + buffer);
				break;
			case MraLogger::DEBUG:
				m_spdlog_logger->debug(m_pretext + buffer);
				break;
			case MraLogger::TRACE:
				m_spdlog_logger->trace(m_pretext + buffer);
				break;
			}
		}
   		va_end(argptr);

	}

}
