#ifndef _MRA_LIBRARIES_LOGGING_SPDLOG_FORMATTER_HPP
#define _MRA_LIBRARIES_LOGGING_SPDLOG_FORMATTER_HPP


#include "spdlog/pattern_formatter.h"
#include "logdebug.hpp"
#include "context.hpp"


class ComponentNameFlagFormatter : public spdlog::custom_flag_formatter
{
public:
    void format(const spdlog::details::log_msg &, const std::tm &, spdlog::memory_buf_t &dest) override
    {
        std::string txt = MRA::Logging::getComponentName();
        dest.append(txt.data(), txt.data() + txt.size());
    }

    std::unique_ptr<custom_flag_formatter> clone() const override
    {
        return spdlog::details::make_unique<ComponentNameFlagFormatter>();
    }
};

std::unique_ptr<::spdlog::pattern_formatter> make_formatter(std::string const& pattern)
{
    LOGDEBUG("make_formatter %s", pattern.c_str());
    auto formatter = std::make_unique<spdlog::pattern_formatter>();
    formatter->add_flag<ComponentNameFlagFormatter>('k');
    formatter->set_pattern(pattern);
    return formatter;
}

#endif // #ifndef _MRA_LIBRARIES_LOGGING_SPDLOG_FORMATTER_HPP

