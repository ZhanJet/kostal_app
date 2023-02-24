/**
 * @file KostalLog.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "KostalLog.hpp"

namespace kostal {

KostalLog * KostalLog::getInstance()
{
    static KostalLog kostalLog;
    return &kostalLog;
}

std::shared_ptr<spdlog::logger> KostalLog::getLogger()
{
    return m_loggerPtr;
}

KostalLog::KostalLog():m_maxSize(g_maxSize),m_filesCount(g_filesCount),m_loggerName(g_loggerName)
{
    m_loggerDir = g_loggerDir + "kostalLog" + getTime();
    m_loggerPtr = spdlog::rotating_logger_mt(m_loggerName, m_loggerDir, m_maxSize, m_filesCount);
}

KostalLog::~KostalLog()
{
    spdlog::drop_all();
}

void KostalLog::info(const std::string& message) const
{
    m_loggerPtr.get()->info(message);
}

void KostalLog::warn(const std::string& message) const
{
    m_loggerPtr.get()->warn(message);
}

void KostalLog::error(const std::string& message) const
{
    m_loggerPtr.get()->error(message);
}

} /* namespace kostal */
