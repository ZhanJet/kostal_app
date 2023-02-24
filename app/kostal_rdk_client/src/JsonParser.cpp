/**
 * @file JsonParser.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "JsonParser.hpp"

namespace kostal {

Status JsonParser::parseSPIJSON(
    std::string* recvMsg, std::string* recvToken, kostal::KostalLog* logPtr)
{
    // if the received message is null
    if (recvMsg == nullptr || recvMsg->size() == 0) {
        k_log->error("The received json message is empty");
        return JSON;
    }

    // if the json reader is failed
    bool result;
    result = m_jsonReader.parse(*recvMsg, m_jsonRecvValue);
    if (!result) {
        k_log->error("The received message is not json format");
        return JSON;
    }

    // check the token first
    if (m_jsonRecvValue.isMember(TOKEN.c_str()) == 0) {
        k_log->error(
            "The received json message does not have initialization param: "
            + TOKEN);
        return JSON;
    }

    // Check whether shakehand message has associated spi params
    if (m_jsonRecvValue.isMember(CPOL.c_str()) == 0) {
        k_log->error(
            "The received json message does not have SPI param: " + CPOL);
        return JSON;
    }
    if (m_jsonRecvValue.isMember(CPHA.c_str()) == 0) {
        k_log->error(
            "The received json message does not have SPI param: " + CPHA);
        return JSON;
    }
    if (m_jsonRecvValue.isMember(LSB.c_str()) == 0) {
        k_log->error(
            "The received json message does not have SPI param: " + LSB);
        return JSON;
    }
    if (m_jsonRecvValue.isMember(SELP.c_str()) == 0) {
        k_log->error(
            "The received json message does not have SPI param: " + SELP);
        return JSON;
    }

    // Retrieve the key value from init json message
    g_CPOL = std::stoi(m_jsonRecvValue[CPOL].asCString());
    g_CPHA = std::stoi(m_jsonRecvValue[CPHA].asCString());
    g_LSB = std::stoi(m_jsonRecvValue[LSB].asCString());
    g_SELPOL = std::stoi(m_jsonRecvValue[SELP].asCString());
    *recvToken = m_jsonRecvValue[TOKEN].asString();

    return SUCCESS;
}

Status JsonParser::parseHBJSON(std::string* recvMsg, std::string* queryStatus,
    std::string* terminateStatus, kostal::KostalLog* logPtr)
{
    // if the received message is null
    if (recvMsg == nullptr || recvMsg->size() == 0) {
        k_log->error("The received json message is empty");
        return JSON;
    }

    // if the json reader is failed
    bool result;
    result = m_jsonReader.parse(*recvMsg, m_jsonRecvValue);
    if (!result) {
        k_log->error("The received message is not json format");
        return JSON;
    }

    // Check whether json has associated members
    if (m_jsonRecvValue.isMember(QUERYSTATUS.c_str()) == 0) {
        k_log->error(
            "The received json message does not have param: " + QUERYSTATUS);
        return JSON;
    }
    if (m_jsonRecvValue.isMember(TERMINATE.c_str()) == 0) {
        k_log->error(
            "The received json message does not have param: " + TERMINATE);
        return JSON;
    }

    // Try to retrieve the key value from json
    *queryStatus = m_jsonRecvValue[QUERYSTATUS].asString();
    *terminateStatus = m_jsonRecvValue[TERMINATE].asString();

    // This can be all commented since it is for shakehand
    k_log->info("****************************************************");
    k_log->info("Check Message received!");
    k_log->info("The received queryStatus is: " + *queryStatus);
    k_log->info("The received terminateStatus is: " + *terminateStatus);
    k_log->info("****************************************************");

    return SUCCESS;
}

Status JsonParser::parseTaskJSON(std::string* recvMsg, std::string* queryStatus,
    std::string* taskType, std::string* taskName, kostal::KostalLog* logPtr)
{
    // if the received message is null
    if (recvMsg == nullptr || recvMsg->size() == 0) {
        k_log->error("The received json message is empty");
        return JSON;
    }

    // if the json reader is failed
    bool result;
    result = m_jsonReader.parse(*recvMsg, m_jsonRecvValue);
    if (!result) {
        k_log->error("The received message is not json format");
        return JSON;
    }

    // Check whether json has associated members
    if (m_jsonRecvValue.isMember(QUERYSTATUS.c_str()) == 0) {
        k_log->error(
            "The received json message does not have param: " + QUERYSTATUS);
        return JSON;
    }
    if (m_jsonRecvValue.isMember(TASKTYPE.c_str()) == 0) {
        k_log->error(
            "The received json message does not have param: " + TASKTYPE);
        return JSON;
    }
    if (m_jsonRecvValue.isMember(TASKNAME.c_str()) == 0) {
        k_log->error(
            "The received json message does not have param: " + TASKNAME);
        return JSON;
    }

    // Try to retrieve the key value from json
    *queryStatus = m_jsonRecvValue[QUERYSTATUS].asString();
    *taskType = m_jsonRecvValue[TASKTYPE].asString();
    *taskName = m_jsonRecvValue[TASKNAME].asString();

    k_log->info("****************************************************");
    k_log->info("Task message received!");
    k_log->info("The received taskType is: " + *taskType);
    k_log->info("The received taskName is: " + *taskName);
    k_log->info("****************************************************");

    return SUCCESS;
}

Status JsonParser::generateSendJSON(std::string* systemStatus,
    std::string* sendMessage, kostal::KostalLog* logPtr)
{
    if (systemStatus == nullptr || systemStatus->size() == 0) {
        k_log->warn("The system status is empty");
        return JSON;
    }
    m_jsonSendValue[SYSTEMSTATUS] = *systemStatus;
    *sendMessage = m_jsonStyledWriter.write(m_jsonSendValue);
    k_log->info("The sending json message is: " + *sendMessage);

    return SUCCESS;
}

} /* namespace kostal */