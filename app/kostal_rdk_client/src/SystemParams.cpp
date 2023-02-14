/**
 * @file SystemParams.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "SystemParams.hpp"

const unsigned short g_COMMPORT = 6070;

std::string ROBOTADDRESS = "127.0.0.1";

std::string LOCALADDRESS = "127.0.0.1";

const int g_MSGMAXSIZE = 1024;

const std::string g_TOKEN = "kostal";

const std::string UPLOADADDRESS = "/home/ftp/";

std::mutex g_kostalDataMutex;

int g_CPHA = 1;

int g_CPOL = 0;

int g_LSB = 0;

int g_SELPOL = 0;

const std::string CPOL = "CPOL";

const std::string CPHA = "CPHA";

const std::string LSB = "LSB";

const std::string SELP = "SELP";

const std::string TOKEN = "TOKEN";

const std::string QUERYSTATUS = "TM_FLEXIV_QUERY_STATUS";

const std::string TASKTYPE = "TM_FLEXIV_TASK_TYPE";

const std::string TASKNAME = "TM_FLEXIV_TASK_NAME";

const std::string TERMINATE = "TM_FLEXIV_TERMINATE";

const std::string SYSTEMSTATUS = "FLEXIV_TM_STATUS";

std::atomic<bool> g_dataCollectFlag = {false};

std::atomic<bool> g_collectSwitch = {false};

int64_t g_timeoutInterval = 5;

const uint8_t g_greyLight = 48;

const uint8_t g_redLight = 49;

const uint8_t g_greenLight = 50;

const uint8_t g_yellowLight = 51;

const uint8_t g_testmanGreyLight = 1001;

const uint8_t g_testmanRedLight = 1002;

const uint8_t g_testmanGreenLight = 1003;

const uint8_t g_testmanYellowLight = 1004;

const uint8_t g_robotGreyLight = 1005;

const uint8_t g_robotRedLight = 1006;

const uint8_t g_robotGreenLight = 1007;

const uint8_t g_robotYellowLight = 1008;

const uint8_t g_spiGreyLight = 1009;

const uint8_t g_spiRedLight = 1010;

const uint8_t g_spiGreenLight = 1011;

const uint8_t g_spiYellowLight = 1012;