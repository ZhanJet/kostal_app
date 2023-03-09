/**
 * @file Communication.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */

#ifndef FLEXIVRDK_COMMUNICATION_HPP_
#define FLEXIVRDK_COMMUNICATION_HPP_

#include "SyncServer.hpp"
#include "SyncTask.hpp"
#include "WriteExcel.hpp"
#include "Participant.hpp"
#include "KostalLog.hpp"

namespace kostal {

/**
 * @class Communications
 * @brief Base class for flexiv system to communicate with outside client
 */
class Communications
{
private:
    // robot data object
    kostal::RobotData m_robotData;

    // spi data object
    kostal::SPIData m_spiData;

    // the list to store robot data
    std::list<kostal::RobotData> m_robotDataList;

    // the list to store spi data
    std::list<kostal::SPIData> m_spiDataList;

    // the list to store left robot data
    std::list<kostal::RobotData> m_leftRobotDataList;

    // the list to store left spi data
    std::list<kostal::SPIData> m_leftSPIDataList;

    // the list to store right robot data
    std::list<kostal::RobotData> m_rightRobotDataList;

    // the list to store left spi data
    std::list<kostal::SPIData> m_rightSPIDataList;

    // m_flexivStatus is inside status of this communication server
    std::atomic<serverStatus> m_flexivStatus = {INIT};

    // m_seriousErrorFlag is the indicator of whether system has a serious
    // error, such as robot CAT0 or SPI failure
    std::atomic<bool> m_seriousErrorFlag = {false};

    // m_checkStatusFlag is the indicator of whether Testman want to check this
    // server's status. It is used to judge whether it is heartbeat or task
    // assignment.
    std::atomic<bool> m_checkStatusFlag = {true};

    // m_terminateSwitch is the indicator of whether Testman want to terminate
    // this system server
    std::atomic<bool> m_terminateSwitch = {false};

    // Kostal's socket service instance
    kostal::SyncServer m_service;

    // flexiv log instance
    flexiv::Log f_log;

    // string variable to store the value of JSON key "TM_FLEXIV_QUERY_STATUS"
    std::string m_queryStatus;

    // string variable to store the value of JSON key "TM_FLEXIV_TASK_TYPE"
    std::string m_taskType;

    // string variable to store the value of JSON key "TM_FLEXIV_TASK_NAME"
    std::string m_taskName;

    // string variable to store the value of JSON key "TM_FLEXIV_TERMINATE"
    std::string m_terminateStatus;

    // thread pool to hold concurrent threads
    boost::asio::thread_pool t_pool;

    // json message handler instance
    kostal::JsonParser m_parserHandler;

    // robot operation handler instance
    kostal::RobotOperations m_robotHandler;

    // sync task handler instance
    kostal::SyncTask m_stHandler;

    // spi operation handler instance
    kostal::SPIOperations m_spiHandler;

    // excel writting handler instance
    kostal::WriteExcel m_weHandler;

    kostal::Participant m_parHandler;

    std::shared_ptr<RdkClientPublisher> m_publisher;

    const std::string topicKostal = "topic_kostal_states";

    KostalMsg pub_msg;

    Node pub_node = Node("kostal_publisher");

public:
    Communications() = default;
    virtual ~Communications() = default;

    /**
     * @brief This function initializes the communication between the server and
     * client, which is called shake-hand. The client needs to send SPI config
     * in this period.
     * @param[in] robotPtr Pointer to robot instance
     * @return Flexiv status code
     */
    Status init(flexiv::Robot* robotPtr);

    /**
     * @brief This function checks the received JSON message and see whether
     * the m_queryStatus is set to true or false. It also check whether the user
     * wants to terminate communication and close the program, the
     * m_terminateSwitch is set to true or false to set
     * @return Flexiv status code
     */
    Status executeCheck();

    /**
     * @brief This function executes the task after the task message is
     * correctly received and parsed
     * @param[in] robotPtr Pointer to robot object
     * @return Flexiv status code
     */
    Status executeTask(flexiv::Robot* robotPtr);

    /**
     * @brief This is a state machine to switch flexiv system from different
     * status, now we have IDLE, BUSY, DONE and FAULT
     * @param[in] robotPtr Pointer to robot object
     * @return Flexiv status code
     */
    void stateMachine(flexiv::Robot* robotPtr);

    /**
     * @brief Return whether the flexiv system is currently in serious error
     * mode if yes, you need to reboot
     * @return true or false
     */
    bool checkSeriousErrorStatus();

    /**
     * @brief Return whether the flexiv system's terminate switch is turned on
     * if yes, close the program and exit
     * @return true or false
     */
    bool checkTerminateStatus();
};

} /* namespace kostal */

#endif /* FLEXIVRDK_COMMUNICATION_HPP_ */