/**
 * @file RobotOperation.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */

#ifndef FLEXIVRDK_ROBOTOPERATIONS_HPP_
#define FLEXIVRDK_ROBOTOPERATIONS_HPP_

#include "KostalStates.hpp"

namespace kostal {

/**
 * @class RobotOperations
 * @brief Base class for robot operations, dealt by a handler
 */
class RobotOperations
{
public:
    RobotOperations() = default;
    virtual ~RobotOperations() = default;

    /**
     * @brief Check robot connection and set robot to plan execution mode
     * @param[in] robotPtr robot's pointer
     * @param[in] logPtr robot's log pointer
     * @return Status code
     */
    Status buildRobotConnection(flexiv::Robot* robotPtr, flexiv::Log* logPtr);

    /**
     * @brief This function helps to clear some tiny software fault that users
     * trigger if return is not success, it means the error can not be
     * eliminated
     * @param[in] robotPtr Pointer to robot object
     * @param[in] logPtr Pointer to flexiv log object
     * @return Flexiv status code
     */
    Status clearTinyFault(flexiv::Robot* robotPtr, flexiv::Log* logPtr);

    /**
     * @brief Access the current robot and spi data and store it into each list
     * @param[in]  robotPtr robot's pointer
     * @param[out] robotDataPtr robot data's pointer
     * @param[out] robotDataListPtr robot data list's pointer
     * @param[out] spiDataPtr spi data's pointer
     * @param[out] spiDataListPtr spi data list's pointer
     * @return Status code
     */
    Status collectSyncData(flexiv::Robot* robotPtr, RobotData* robotDataPtr,
        std::list<RobotData>* robotDataListPtr);

    /**
     * @brief Check whether robot has this plan in list
     * @param[in] robotPtr robot's pointer
     * @param[in] logPtr robot's log pointer
     * @param[in] planName the name of the executing work plan
     * @return yes for do have, no for do not have
     */
    bool checkRobotPlan(
        flexiv::Robot* robotPtr, flexiv::Log* logPtr, std::string planName);

    /**
     * @brief Execute the plan in robot by name
     * @param[in] robotPtr robot's pointer
     * @param[in] logPtr robot's log pointer
     * @param[in] planName the name of the executing work plan
     * @return Status code
     */
    Status executeRobotPlan(
        flexiv::Robot* robotPtr, flexiv::Log* logPtr, std::string planName);
};

} /* namespace kostal */

#endif /* FLEXIVRDK_ROBOTOPERATION_HPP_ */
