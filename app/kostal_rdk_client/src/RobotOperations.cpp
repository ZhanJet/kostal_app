/**
 * @file RobotOperations.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "RobotOperations.hpp"

namespace kostal {

Status RobotOperations::buildRobotConnection(
    flexiv::Robot* robotPtr, flexiv::Log* logPtr)
{
    // Check whether the robot is connected or not
    if (robotPtr->isConnected() != true) {
        logPtr->error("The robot is not connected");
        return ROBOT;
    } else
        logPtr->info("The robot is now connected");

    // Clear fault on robot server if any
    if (robotPtr->isFault()) {
        logPtr->warn("Robot is facing a fault, trying to clear the fault...");

        // Try to clear the fault
        robotPtr->clearFault();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Check fault again
        if (robotPtr->isFault()) {
            logPtr->error(
                "===================================================");
            logPtr->error("Robot's fault cannot be cleared, exiting ...");

            return ROBOT;
        }
        logPtr->info("Fault on robot server is cleared");
    }
    try {
        // robotPtr->enable();
    } catch (const flexiv::Exception& e) {
        logPtr->error(e.what());
        logPtr->error("The robot can not be enabled");
        return ROBOT;
    }

    // Wait for the robot to become operational
    while (!robotPtr->isOperational()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    logPtr->info("The robot is now operational");

    // Set mode after robot is operational
    robotPtr->setMode(flexiv::MODE_PLAN_EXECUTION);

    // Wait for the mode to be switched
    while (robotPtr->getMode() != flexiv::MODE_PLAN_EXECUTION) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    logPtr->info("The robot is now in plan execution mode");

    return SUCCESS;
}

Status RobotOperations::clearTinyFault(
    flexiv::Robot* robotPtr, flexiv::Log* logPtr)
{
    // Clear fault on robot server if any
    if (robotPtr->isFault()) {
        logPtr->warn("Robot is facing a fault, trying to clear the fault...");
        // Try to clear the fault
        robotPtr->clearFault();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        // Check again
        if (robotPtr->isFault()) {
            logPtr->error(
                "===================================================");
            logPtr->error("Robot's fault cannot be cleared, exiting ...");
            return ROBOT;
        } else {
            logPtr->info("Fault on robot server is cleared");
        }
    }

    return SUCCESS;
}

Status RobotOperations::collectSyncData(flexiv::Robot* robotPtr,
    RobotData* robotDataPtr, std::list<RobotData>* robotDataListPtr,
    SPIData* spiDataPtr, std::list<SPIData>* spiDataListPtr)
{
    while (g_collectSwitch) {
        // get plan info
        flexiv::PlanInfo planInfo;
        robotPtr->getPlanInfo(planInfo);

        // start collecting when nodename is "Start"
        if (planInfo.nodeName == "Start") {
            g_dataCollectFlag = true;
        }

        // stop collecting when nodename is "Stop"
        if (planInfo.nodeName == "Stop") {
            g_dataCollectFlag = false;
        }

        // only collect when collect flag is on
        if (g_dataCollectFlag == true) {
            robotDataPtr->nodeName = planInfo.nodeName;

            // get robot states
            flexiv::RobotStates robotStates;
            {
                // use mutex to lock data inputting step
                std::lock_guard<std::mutex> lock(g_kostalDataMutex);

                // get robot states and input them to robot data instance
                robotPtr->getRobotStates(robotStates);
                robotDataPtr->tcpPose = robotStates.tcpPose;
                robotDataPtr->rawDataForceSensor
                    = robotStates.extWrenchInTcp;
                robotDataPtr->flangePose = robotStates.flangePose;

                // put data instance to list
                robotDataListPtr->push_back(*robotDataPtr);
                spiDataListPtr->push_back(*spiDataPtr);
            }
        }
    }

    return SUCCESS;
}

bool RobotOperations::checkRobotPlan(
    flexiv::Robot* robotPtr, flexiv::Log* logPtr, std::string planName)
{
    auto planList = robotPtr->getPlanNameList();

    if (std::find(planList.begin(), planList.end(), planName)
        == planList.end()) {
        logPtr->error("=================================================");
        logPtr->error("The robot does not have planName: " + planName);
        return false;
    }
    // if exists, return true
    return true;
}

Status RobotOperations::executeRobotPlan(
    flexiv::Robot* robotPtr, flexiv::Log* logPtr, std::string planName)
{
    // execute plan by name
    robotPtr->executePlanByName(planName);

    // sleep when robot is prepare to execute
    while (robotPtr->isBusy()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return SUCCESS;
}

} /* namespace kostal */
