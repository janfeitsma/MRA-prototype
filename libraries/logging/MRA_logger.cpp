#include "MRA_logger.hpp"
#include <memory>
#include "spdlog/spdlog.h"  // spdlog API: https://github.com/gabime/spdlog
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"

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
    m_spdlog_logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "/tmp/async_log.txt");
}

void MraLogger::logStart(double timestamp, int counter,
		const std::string& r_input_str, const std::string& r_params_str, const std::string& r_state_str)
{
	if (m_active) {
		m_spdlog_logger->info("tick {} START", counter);
		m_spdlog_logger->info("timestamp: {}", timestamp);
		m_spdlog_logger->info("input: {}", r_input_str);
		m_spdlog_logger->info("params: {}", r_params_str);
		m_spdlog_logger->info("state: {}", r_state_str);
	}
}

void MraLogger::logEnd(int counter, int *p_err, double duration, const std::string& r_output_str, const std::string& r_state_str)
{
	if (m_active) {
		m_spdlog_logger->info("tick {} END error_value={}", counter, *p_err);
		m_spdlog_logger->info("duration: {}", duration);
		m_spdlog_logger->info(" output: {}", r_output_str);
		m_spdlog_logger->info("state: {}", r_state_str);
	}
}
