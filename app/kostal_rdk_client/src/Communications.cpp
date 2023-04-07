/**
 * @file Communication.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "Communications.hpp"

namespace kostal {

Status Communications::init(flexiv::Robot* robotPtr)
{
    // start publishing at the init stage
    m_publisher = m_parHandler.createPublisher(&pub_node, topicKostal);

    // m_parHandler.publishMsg(m_publisher, &pub_msg, g_greyLight);
    m_parHandler.publishTestmanMsg(m_publisher, &pub_msg, g_testmanGreyLight);
    m_parHandler.publishRobotMsg(m_publisher, &pub_msg, g_robotGreyLight);
    m_parHandler.publishSpiMsg(m_publisher, &pub_msg, g_spiGreyLight);

    // sleep 2 seconds and wait for subscriber
    std::this_thread::sleep_for(std::chrono::seconds(2));
    // m_parHandler.publishMsg(m_publisher, &pub_msg, g_greenLight);

    Status result;

    // 1st, check robot connection and set robot to plan execution mode, if
    // failed, continue to init socket server and send error status to client
    result = m_robotHandler.buildRobotConnection(robotPtr, &f_log);
    if (result != SUCCESS) {
        // If failed at the first place, need to report and exit
        k_log->error("The flexiv system failed to initialize the robot!");
        k_log->error("Please recover the robot and then reboot it");
        k_log->error("===================================================");
        m_flexivStatus = FAULT;
        m_seriousErrorFlag = true;
        // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);
        m_parHandler.publishRobotMsg(m_publisher, &pub_msg, g_robotRedLight);
    } else {
        k_log->info("The robot connection is built successfully");
        m_parHandler.publishRobotMsg(m_publisher, &pub_msg, g_robotGreenLight);
    }
    k_log->info("Conect with testman...");

    // 2nd, try to initialize the socket server and get spi config from
    // testman's shakehand message
    result = m_service.init();
    if (result != SUCCESS) {
        k_log->error(
            "The flexiv system fails to initialize the socket server, "
            "please check your network");
        // if socket failed, directly return error code and break because
        // there is no need to continue
        m_flexivStatus = FAULT;
        m_seriousErrorFlag = true;
        // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);
        m_parHandler.publishTestmanMsg(m_publisher, &pub_msg, g_testmanRedLight);

        return result;
    } else {
        k_log->info("The socket connection is built successfully");
        m_parHandler.publishTestmanMsg(m_publisher, &pub_msg, g_testmanGreenLight);
    }

    // 3rd, check spi device connection with the spi config in shake-hand
    // message
    result = m_spiHandler.buildSPIConnectionSocket();
    m_parHandler.publishMsg(m_publisher, &pub_msg);
    if (result != SUCCESS) {
        m_flexivStatus = FAULT;
        m_seriousErrorFlag = true;
        m_parHandler.publishSpiMsg(m_publisher, &pub_msg, g_spiRedLight);
    } else {
        k_log->info("The spi connection is built successfully");
        m_parHandler.publishSpiMsg(m_publisher, &pub_msg, g_spiGreenLight);
    }

    // 4th, if flexivStatus has been changed from INIT, but the socket server
    // is successfully initialized, return SYSTEM to indicate a hardware fault.
    if (m_flexivStatus != INIT) {
        k_log->error(
            "The flexiv system failed in initialization, please check "
            "ROBOT or SPI");
        m_flexivStatus = FAULT;
        // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);

        return SYSTEM;
    }

    // 5th, if flexivStatus still keeps at INIT, return success and switch
    // flexivStatus to IDLE
    k_log->info("The flexiv system is initialized successfully");
    m_flexivStatus = IDLE;
    m_parHandler.publishMsg(m_publisher, &pub_msg, g_greenLight);

    return SUCCESS;
}

Status Communications::executeCheck()
{
    Status result;

    // get received message first
    std::string recvMsg = m_service.getRecvMsg();

    // parse the json message to retrieve the value of queryStatus and
    // terminateStatus
    result = m_parserHandler.parseHBJSON(
        &recvMsg, &m_queryStatus, &m_terminateStatus, k_log);
    if (result != SUCCESS) {
        m_flexivStatus = FAULT;
        k_log->error("The task message is failed to be parsed");
        // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);
        m_parHandler.publishTestmanMsg(m_publisher, &pub_msg, g_testmanRedLight);


        return result;
    }

    // Check the terminateStatus and turn on the terminate switch if the answer
    // is True or Yes
    if (m_terminateStatus == "true" || m_terminateStatus == "True"
        || m_terminateStatus == "TRUE" || m_terminateStatus == "yes"
        || m_terminateStatus == "Yes" || m_terminateStatus == "YES") {
        m_terminateSwitch = true;
    } else {
        m_terminateSwitch = false;
    }

    // Check the queryStatus and turn on the check status flag if the answer is
    // True or Yes
    if (m_queryStatus == "true" || m_queryStatus == "True"
        || m_queryStatus == "TRUE" || m_queryStatus == "yes"
        || m_queryStatus == "Yes" || m_queryStatus == "YES") {
        m_checkStatusFlag = true;
    } else {
        m_checkStatusFlag = false;
    }

    return SUCCESS;
}

Status Communications::executeTask(flexiv::Robot* robotPtr)
{
    Status result;

    // 1st, get received message first
    std::string recvMsg = m_service.getRecvMsg();

    // 2nd, parse the json message to retrieve the value of queryStatus,
    // taskType and taskName
    result = m_parserHandler.parseTaskJSON(
        &recvMsg, &m_queryStatus, &m_taskType, &m_taskName, k_log);
    if (result != SUCCESS) {
        m_flexivStatus = FAULT;
        k_log->error("The task message is failed to be parsed");
        k_log->error("===================================================");
        // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);
        m_parHandler.publishTestmanMsg(m_publisher, &pub_msg, g_testmanRedLight);


        return result;
    }
    m_parHandler.publishMsg(m_publisher, &pub_msg, &m_taskName, &m_taskType);

    // 3rd, before executing the task, we need to clear these two lists manually
    m_spiDataList.clear();
    m_robotDataList.clear();

    // 4th, run a sync task to execution the workplan and collect the robot data
    // at the same time
    result = m_stHandler.runScheduler(robotPtr, &m_spiData, &m_spiDataList,
        &m_robotData, &m_robotDataList, &f_log, m_taskName + "-" + m_taskType + "-MainPlan");
    if (result != SUCCESS) {
        m_flexivStatus = FAULT;
        k_log->error("The sync task is failed to be executed");
        k_log->error("===================================================");
        // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);

        return result;
    }

    // result = m_weHandler.writeTwoExcel(m_taskType, m_taskName,
    // &m_robotDataList, &m_spiDataList, &m_leftRobotDataList,
    // &m_leftSPIDataList, &m_rightRobotDataList, &m_rightSPIDataList,
    //&f_log);

    // 5th, write excel files with collected robot data list and spi data list
    result = m_weHandler.writeDataToExcel(
        m_taskType, m_taskName, &m_robotDataList, &m_spiDataList, &f_log);
    if (result != SUCCESS) {
        m_flexivStatus = FAULT;
        k_log->error("The excel file is failed to be generated");
        k_log->error("===================================================");
        // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);

        return result;
    }

    // 6th, after all these steps, mention that the task execution is finished
    k_log->info("****************************************************");
    k_log->info("The task is executed successfully");
    k_log->info("****************************************************");

    // Very Important!!
    // 7th, try to reinit the spi device before next round, the spi device is
    // found to be blocked if we do not initialize it manually
    k_log->info("Try to reinitialize the SPI device...");
    result = m_spiHandler.reinitSPI();
    if (result != SUCCESS) {
        m_flexivStatus = FAULT;
        k_log->error("The SPI re-initialization is failed");
        k_log->error("===================================================");
        // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);
        m_parHandler.publishSpiMsg(m_publisher, &pub_msg, g_spiRedLight);

        return result;
    } else {
        k_log->info("The SPI device is reinitialized successfully");
    }

    // 8th, after the reinit of the spi device, switch flexivStatus to IDLE
    // again
    m_flexivStatus = IDLE;

    return SUCCESS;
}

void Communications::stateMachine(flexiv::Robot* robotPtr)
{
    Status result;

    // put all the status transition into a closed-loop Finite State Machine
    while (true) {
        // the transition condition is flexivStatus
        switch (m_flexivStatus) {
            case IDLE: {
                // first, we check whether robot has tiny fault, if yes, clear
                // it. If it fails, jump out of the loop
                result = m_robotHandler.clearTinyFault(robotPtr, &f_log);
                if (result != SUCCESS) {
                    m_flexivStatus = FAULT;
                    m_seriousErrorFlag = true;
                    k_log->error("Please recover the robot and then reboot it");
                    // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);
                    m_parHandler.publishRobotMsg(m_publisher, &pub_msg, g_robotRedLight);
                    return;
                }

                // manually set these two indicators to default value.
                m_checkStatusFlag = true;
                m_terminateSwitch = false;

                // when m_checkStatusFlag is equal to true, it will stay in this
                // loop
                while (m_checkStatusFlag == true) {

                    // This will be print all over the heartbeat
                    k_log->info(
                        "The flexiv system is in idle mode, listening...");

                    // set reply msg to "IDLE" and keep answering
                    m_service.setReplyMsg("IDLE");
                    result = m_service.monitor();
                    if (result != SUCCESS) {
                        m_flexivStatus = FAULT;
                        k_log->error(
                            "The flexiv system is having an error in "
                            "heartbeat");
                        k_log->error(
                            "=============================================="
                            "=====");
                        // m_parHandler.publishMsg(
                            //m_publisher, &pub_msg, g_redLight);
                        m_parHandler.publishTestmanMsg(
                            m_publisher, &pub_msg, g_testmanRedLight);

                        return;
                    }

                    // check the param and update the m_checkStatusFlag
                    result = executeCheck();
                    if (result != SUCCESS) {
                        m_flexivStatus = FAULT;
                        k_log->error(
                            "The flexiv system is having an error in "
                            "querying server status");
                        // m_parHandler.publishMsg(
                            // m_publisher, &pub_msg, g_redLight);
                        m_parHandler.publishTestmanMsg(
                            m_publisher, &pub_msg, g_testmanRedLight);

                        return;
                    }
                }

                // when m_checkStatusFlag is not equal to true, which means
                // it is time to execute task

                // Check whether client sends terminate signal
                // if the client does not require for terminate
                if (m_terminateSwitch == false) {
                    // set status to BUSY and put the task into a thread
                    // pool
                    k_log->info("The flexiv system received a task!");
                    m_flexivStatus = BUSY;
                    boost::asio::post(
                        t_pool, boost::bind(&Communications::executeTask, this,
                                    robotPtr));
                    // directly go to BUSY in switch-case
                    break;
                } else {
                    // client is sending a terminate signal, so go to
                    // switch-case DONE directly
                    k_log->info(
                        "The flexiv system received a terminate signal!");
                    m_parHandler.publishTestmanMsg(m_publisher, &pub_msg, g_testmanGreyLight);
                    m_parHandler.publishRobotMsg(m_publisher, &pub_msg, g_robotGreyLight);
                    m_parHandler.publishSpiMsg(m_publisher, &pub_msg, g_spiGreyLight);
                    m_flexivStatus = DONE;
                    break;
                }
            }

            case BUSY: {
                // change light to yellow to hint busy
                m_parHandler.publishMsg(m_publisher, &pub_msg, g_yellowLight);

                // set reply message to "BUSY"
                m_service.setReplyMsg("BUSY");

                // listen and reply busy to testman
                result = m_service.monitor();
                if (result != SUCCESS) {
                    k_log->error(
                        "The flexiv system is having an error in "
                        "connection, wait for task to be finished...");
                    k_log->error(
                        "=================================================="
                        "=");
                    // if socket fails, wait for current task to finish and
                    // set status to fault
                    t_pool.join();
                    m_flexivStatus = FAULT;
                    // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);
                    m_parHandler.publishTestmanMsg(m_publisher, &pub_msg, g_testmanRedLight);
                    return;
                }
                break;
            }

            case DONE: {
                k_log->info(
                    "===================================================");
                k_log->info("The flexiv system is turning off...");
                k_log->info(
                    "===================================================");
                // just close the socket service
                m_service.disconnect();
                k_log->info(
                    "Flexiv system server closed this connection safely");
                return;
            }

            // this status is actually not used, since socket error cause
            // return
            case FAULT: {
                m_service.setReplyMsg("FAULT");
                k_log->error("The flexiv system is in fault mode...");
                k_log->error(
                    "===================================================");
                // set reply content to "FAULT" and reply to client
                result = m_service.monitor();
                if (result != SUCCESS) {
                    k_log->error(
                        "The flexiv system is having an error in "
                        "fault status");
                    k_log->error(
                        "=================================================="
                        "=");
                    m_flexivStatus = FAULT;
                    // m_parHandler.publishMsg(m_publisher, &pub_msg, g_redLight);
                    m_parHandler.publishTestmanMsg(m_publisher, &pub_msg, g_testmanRedLight);
                    m_parHandler.publishRobotMsg(m_publisher, &pub_msg, g_robotGreyLight);
                    m_parHandler.publishSpiMsg(m_publisher, &pub_msg, g_spiGreyLight);

                    return;
                }
                m_service.disconnect();
                k_log->error(
                    "Flexiv system server closed this connection due to "
                    "fault");
                return;
            }
        }
    }
}

bool Communications::checkSeriousErrorStatus()
{
    if (m_seriousErrorFlag == true) {
        return true;
    } else {
        return false;
    }
}

bool Communications::checkTerminateStatus()
{
    if (m_terminateSwitch == true) {
        return true;
    } else {
        return false;
    }
}

} /* namespace kostal */
