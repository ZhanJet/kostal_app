/**
 * @file RobotOperations.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */

#ifndef _ROBOT_OPERATIONS_HPP_
#define _ROBOT_OPERATIONS_HPP_

// Flexiv header files
#include <flexiv/Exception.hpp>
#include <flexiv/Log.hpp>
#include <flexiv/Mode.hpp>
#include <flexiv/Robot.hpp>
#include <flexiv/StatesData.hpp>

namespace flexiv {

/**
 * @class RobotOperations
 * @brief This class represent robot's major operations to get prepared or
 * switch mode
 */
class RobotOperations
{
public:
    RobotOperations() = default;
    virtual ~RobotOperations() = default;
    /**
     * @brief This function helps to clear some tiny software fault
     * @param[in] robot robot's pointer
     * @param[in] log log's pointer
     * @return error return 1, success return 0
     */
    int clearTinyFault(flexiv::Robot* robot, flexiv::Log* log);

    /**
     * @brief Prepare the robot by checking any fault. If fault exists, clear
     * them. If still fault, return error
     * @param[in] robot robot's pointer
     * @param[in] log log's pointer
     * @return error return 1, success return 0
     */
    int prepareRobot(flexiv::Robot* robot, flexiv::Log* log);

    /**
     * @brief Set the robot to plan execution mode
     * @param[in] robot robot's pointer
     * @param[in] log log's pointer
     */
    void setExecutionMode(flexiv::Robot* robot, flexiv::Log* log);

    /**
     * @brief Fetch robot states and put them into outside arrays and sleep 1
     * seconds
     * @param[in] robot robot's pointer
     * @param[in] tcpPose_array array to store tcpPose
     * @param[in] flangePose_array array to store flangePose
     * @param[in] rawForceSensor_array array to store rawForceSensorData (Not
     * used yet)
     */
    void fetchRobotStates(flexiv::Robot* robot,
        std::array<double, 7>* tcpPose_array,
        std::array<double, 7>* flangePose_array,
        std::array<double, 6>* rawForceSensor_array);

    /**
     * @brief Execute the plan in robot by name
     * @param[in] robot robot's pointer
     * @param[in] log log's pointer
     * @param[in] planName the pointer of the work plan name to be executed
     * @param[in] startExec the switch of whether robot should start plan
     * execution mode
     * @param[in] stopExec the switch of whether robot should stop plan
     * execution mode
     */
    void executeRobotPlan(flexiv::Robot* robot, flexiv::Log* log,
        std::string* planName, bool* startExec, bool* stopExec);
};
}

#endif // _ROBOT_OPERATIONS_HPP_