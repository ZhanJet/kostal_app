/**
 * @file KostalLog.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */

#ifndef FLEXIVRDK_KOSTALLOG_HPP_
#define FLEXIVRDK_KOSTALLOG_HPP_

#include "SystemParams.hpp"

namespace kostal {

/**
 * @class KostalLog
 * @brief Loggers to print messages with timestamp, for kostal project only
 */
class KostalLog
{
public:
    KostalLog() = default;
    virtual ~KostalLog() = default;

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
};

} /* namespace kostal */

#endif /* FLEXIVRDK_KOSTALLOG_HPP_ */
