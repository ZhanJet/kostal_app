/**
 * @file KostalLog.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "KostalLog.hpp"

std::shared_ptr<spdlog::logger> file_logger = spdlog::basic_logger_mt("file_logger", "filename.log");

namespace kostal {

void KostalLog::info(const std::string& message) const
{
    file_logger.get()->info(message);
}

void KostalLog::warn(const std::string& message) const
{
    file_logger.get()->warn(message);
}

void KostalLog::error(const std::string& message) const
{
    file_logger.get()->error(message);
}

} /* namespace kostal */
