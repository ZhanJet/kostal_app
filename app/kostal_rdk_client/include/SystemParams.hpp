/**
 * @file SystemParams.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 * @author chenchen.liu@flexiv.com
 */

#ifndef FLEXIVRDK_SYSTEMPARAMS_HPP_
#define FLEXIVRDK_SYSTEMPARAMS_HPP_

// Flexiv header files
#include <flexiv/Exception.hpp>
#include <flexiv/Gripper.hpp>
#include <flexiv/Log.hpp>
#include <flexiv/Mode.hpp>
#include <flexiv/Model.hpp>
#include <flexiv/Robot.hpp>
#include <flexiv/Scheduler.hpp>
#include <flexiv/StatesData.hpp>
#include <flexiv/Utility.hpp>

// STL header files
#include <iostream>
#include <string>
#include <cstring>
#include <mutex>
#include <list>
#include <vector>
#include <array>
#include <algorithm>
#include <thread>
#include <iomanip>
#include <fstream>
#include <future>
#include "math.h"

// 3rd-party libs
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/asio/thread_pool.hpp>
#include <jsoncpp/json/json.h>
#include <spdlog/spdlog.h>
#include <eigen3/Eigen/Eigen>

// The port number of destination host
extern const unsigned short g_COMMPORT;

// Robot IP Address
extern std::string ROBOTADDRESS;

// Local IP Address
extern std::string LOCALADDRESS;

// Max length of the socket message
extern const int g_MSGMAXSIZE;

// Token for shake-hand with kostal testman
extern const std::string g_TOKEN;

// The file stored location prefix
extern const std::string UPLOADADDRESS;

// Default spi config
extern int g_CPHA;
extern int g_CPOL;
extern int g_LSB;
extern int g_SELPOL;

// The global variant status shows the status of the system, 0 means success,
// 1-4 means errors in different periods
enum Status
{
    SUCCESS,
    SOCKET,
    JSON,
    ROBOT,
    SPI,
    CSV,
    FTP,
    SYSTEM
};

// The status of the flexiv server
enum serverStatus
{
    INIT,
    IDLE,
    BUSY,
    FAULT,
    DONE
};

// The status of the status light
extern const uint8_t g_greyLight;
extern const uint8_t g_redLight;
extern const uint8_t g_greenLight;
extern const uint8_t g_yellowLight;

// global mutext for spi data collection and robot data collection
extern std::mutex g_kostalDataMutex;

// SPI config key that testman will send
extern const std::string CPOL;
extern const std::string CPHA;
extern const std::string LSB;
extern const std::string SELP;
extern const std::string TOKEN;

// Json message key of query status
extern const std::string QUERYSTATUS;

// Json message key of task type
extern const std::string TASKTYPE;

// Json message key of task name
extern const std::string TASKNAME;

// Json message key of terminate status
extern const std::string TERMINATE;

// Json message key of flexiv system status
extern const std::string SYSTEMSTATUS;

// Whether the node data should be collected or not
extern std::atomic<bool> g_dataCollectFlag;

// Whether the whole collecting logic should be used or not
extern std::atomic<bool> g_collectSwitch;

// Connection timeout interval after first handshake with Testman, unit is
// second
extern int64_t g_timeoutInterval;

#endif /* FLEXIVRDK_SYSTEMPARAMS_HPP_ */