/**
 * @file KostalLog.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "KostalLog.hpp"

namespace kostal {

void KostalLog::info(const std::string& message) const
{
    spdlog::info(message);
}

void KostalLog::warn(const std::string& message) const
{
    spdlog::warn(message);
}

void KostalLog::error(const std::string& message) const
{
    spdlog::error(message);
}

} /* namespace kostal */
