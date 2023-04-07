/**
 * @file SystemParams.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "SystemParams.hpp"

const unsigned short g_COMMPORT = 6070;

std::string ROBOTADDRESS = "192.168.10.10";

std::string LOCALADDRESS = "192.168.10.11";

const int g_MSGMAXSIZE = 1024;

const std::string g_TOKEN = "kostal";

const std::string UPLOADADDRESS = "/home/kostal/";

std::mutex g_kostalDataMutex;

int g_CPHA = 0;

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

const uint8_t g_testmanGreyLight = 101; //断开

const uint8_t g_testmanRedLight = 102; //错误

const uint8_t g_testmanGreenLight = 103; //连接

const uint8_t g_testmanYellowLight = 104; //运行

const uint8_t g_robotGreyLight = 105;

const uint8_t g_robotRedLight = 106;

const uint8_t g_robotGreenLight = 107;

const uint8_t g_robotYellowLight = 108;

const uint8_t g_spiGreyLight = 109;

const uint8_t g_spiRedLight = 110;

const uint8_t g_spiGreenLight = 111;

const uint8_t g_spiYellowLight = 112;

std::string g_loggerName = "kostalLog";

std::string g_loggerDir = "/home/kostal/logfile/";

const int g_maxSize = 10*1024*1024;

const int g_filesCount = 100;