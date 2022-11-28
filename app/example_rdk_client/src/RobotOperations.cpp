/**
 * @file RobotOperations.cpp
 * @date Nov 24, 2022
 * @author Chenchen Liu
 */

#include "RobotOperations.hpp"
#include <thread>
#include <algorithm>

namespace flexiv {

int RobotOperations::clearTinyFault(flexiv::Robot* robot, flexiv::Log* log)
{
    // Clear fault on robot server if any
    if (robot->isFault()) {
        log->warn("Robot is facing a fault, trying to clear the fault...");

        // Try to clear the fault
        robot->clearFault();
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // Check the fault again
        if (robot->isFault()) {
            log->error("Fault cannot be cleared, exiting ...");
            return 1;
        } else {
            log->info("Fault on robot server is cleared");
        }
    }

    return 0;
}

int RobotOperations::prepareRobot(flexiv::Robot* robot, flexiv::Log* log)
{
    // try  to clear tiny fault first
    int status = clearTinyFault(robot, log);
    if (status == 1) {
        return 1;
    }
    log->info("Enabling robot ...");
    // robot.enable();
    //  Wait for the robot to become operational
    int secondsWaited = 0;
    while (!robot->isOperational()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (++secondsWaited == 10) {
            log->warn(
                "Still waiting for robot to become operational, please "
                "check that the robot 1) has no fault, 2) is booted "
                "into Auto mode");
        }
    }
    log->info("Robot is now operational");

    return 0;
}

void RobotOperations::setExecutionMode(flexiv::Robot* robot, flexiv::Log* log)
{
    // Check whether robot is operation or not
    if (!robot->isOperational()) {
        log->error("Robot is not operational, please reboot...");
        return;
    }

    // Set mode to plan execution after robot is operational
    robot->setMode(flexiv::MODE_PLAN_EXECUTION);

    // Wait for the mode to be switched
    while (robot->getMode() != flexiv::MODE_PLAN_EXECUTION) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    return;
}

void RobotOperations::fetchRobotStates(flexiv::Robot* robot,
    std::array<double, 7>* tcpPose_array,
    std::array<double, 7>* flangePose_array,
    std::array<double, 6>* rawForceSensor_array)
{
    // Data struct for storing robot states
    flexiv::RobotStates robotStates;

    // Get robot states and output
    robot->getRobotStates(robotStates);

    // Retrieve 7 numbers and put them into outside arrays
    std::copy_n(robotStates.tcpPose.begin(), 7, tcpPose_array->begin());
    std::copy_n(robotStates.flangePose.begin(), 7, flangePose_array->begin());

    // Sleep for 1 seconds
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return;
}

void RobotOperations::executeRobotPlan(flexiv::Robot* robot, flexiv::Log* log,
    std::string* planName, bool* startExec, bool* stopExec)
{
    // 1st, check the status. If error, just return
    int status = clearTinyFault(robot, log);
    if (status == 1) {
        return;
    }

    // 2nd, check start execution and plan name
    if (*startExec) {
        setExecutionMode(robot, log);
        if (*planName == "") {
            log->info("Robot is now in plan execution mode");
            return;
        }
    }

    // 3rd, check stop sign
    if (*stopExec) {
        log->info("Stop the robot and transit to idle mode");
        robot->stop();
        return;
    }

    // 4th, check whether this plan exists. If no, just return
    auto planList = robot->getPlanNameList();
    if (std::find(planList.begin(), planList.end(), *planName)
        == planList.end()) {
        log->warn("Plan: " + *planName + " does not exist");
        return;
    }

    // 5th, announce and execute plan by name
    log->info("Executing plan: " + *planName + "...");
    robot->executePlanByName(*planName);

    // Sleep while plan is running
    while (robot->isBusy()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
}