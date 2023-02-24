/**
 * @file KostalLog.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */

#ifndef FLEXIVRDK_KOSTALLOG_HPP_
#define FLEXIVRDK_KOSTALLOG_HPP_

#include "SystemParams.hpp"

#if defined (k_log) 
#undef k_log
#endif
#define k_log (kostal::KostalLog::getInstance())

namespace kostal {

/**
 * @class KostalLog
 * @brief Loggers to print messages with timestamp, for kostal project only
 */

 // get current time in time_t format
extern std::string getTime();

class KostalLog
{
public:

    /**
     * @brief Get KostalLog instance by only header
     * @return KostalLog instance 
     */
    static KostalLog * getInstance();

    /**
     * @brief Get KostalLog pointer of rotating_logger_mt
     * @return KostalLog pointer
     */
    std::shared_ptr<spdlog::logger> getLogger();

    /**
     * @brief Print info message with timestamp and coloring
     * @param[in] message Info message
     * @note Color = green
     */
    void info(const std::string& message) const;

    /**
     * @brief Print warning message with timestamp and coloring
     * @param[in] message Warning message
     * @note Color = yellow
     */
    void warn(const std::string& message) const;

    /**
     * @brief Print error message with timestamp and coloring
     * @param[in] message Error message
     * @note Color = red
     */
    void error(const std::string& message) const;

private:
    KostalLog();
    virtual ~KostalLog();
    KostalLog(const KostalLog &) = delete;
    KostalLog & operator=(const KostalLog &) = delete;
    std::shared_ptr<spdlog::logger> m_loggerPtr;
    std::string m_loggerName;
    std::string m_loggerDir;
    const int m_maxSize;
    const int m_filesCount;

};

} /* namespace kostal */

#endif /* FLEXIVRDK_KOSTALLOG_HPP_ */
