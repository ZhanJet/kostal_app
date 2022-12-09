/**
 * @file SyncTask.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "SyncTask.hpp"

namespace kostal {

Status SyncTask::runScheduler(flexiv::Robot* robotPtr, SPIData* spiDataPtr,
    std::list<SPIData>* spiDataListPtr, RobotData* robotDataPtr,
    std::list<RobotData>* robotDataListPtr, flexiv::Log* logPtr,
    std::string planName)
{
    Status result;

    // check whether robot has this plan name first
    if (m_robotHandler.checkRobotPlan(robotPtr, logPtr, planName) == false) {
        return ROBOT;
    }

    // turn on the switch to collect data
    g_collectSwitch = true;

    // execute the plan by planName
    robotPtr->executePlanByName(planName);

    // Create a task pool to let two data collect threads run
    boost::asio::thread_pool collectTaskPool;

    // Insert SPI data collect thread
    boost::asio::post(
        collectTaskPool, boost::bind(&kostal::SPIOperations::collectSPIData,
                             m_spiHandler, spiDataPtr, logPtr));

    // Insert Robot data collect thread
    boost::asio::post(
        collectTaskPool, boost::bind(&kostal::RobotOperations::collectSyncData,
                             m_robotHandler, robotPtr, robotDataPtr,
                             robotDataListPtr, spiDataPtr, spiDataListPtr));

    // Wait until the program is finished
    while (robotPtr->isBusy()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Close the two while-loop in threads
    {
        std::lock_guard<std::mutex> lock(g_kostalDataMutex);
        g_collectSwitch = false;
    }

    // Wait for threads till the end
    collectTaskPool.join();
    logPtr->info("The sync task is finished by scheduler");

    return SUCCESS;
}

} /* namespace kostal */