/**
 * @file SyncTask.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "SyncTask.hpp"
#include <thread>
#include <functional>

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

    auto spiWorker = std::bind(&kostal::SPIOperations::collectSPIData, m_spiHandler, spiDataPtr, spiDataListPtr, logPtr);
    auto robotWorker = std::bind(&kostal::RobotOperations::collectSyncData, m_robotHandler, robotPtr, robotDataPtr, robotDataListPtr);

    std::thread robotThread(robotWorker);
    std::thread spiThread(spiWorker);

    // Wait until the program is finished
    while (robotPtr->isBusy()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    g_collectSwitch = false;

    // Wait for threads till the end
    robotThread.join();
    spiThread.join();
    
    k_log->info("The sync task is finished by scheduler");

    return SUCCESS;
}

} /* namespace kostal */
