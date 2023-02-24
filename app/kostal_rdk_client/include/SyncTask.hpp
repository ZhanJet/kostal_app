/**
 * @file RobotOperation.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */

#ifndef FLEXIVRDK_SYNCTASK_HPP_
#define FLEXIVRDK_SYNCTASK_HPP_

#include "SPIOperations.hpp"
#include "RobotOperations.hpp"
#include "KostalLog.hpp"

namespace kostal {

/**
 * @class SyncTask
 * @brief Base class for run sync task
 */
class SyncTask
{
private:
    kostal::RobotOperations m_robotHandler;
    kostal::SPIOperations m_spiHandler;

public:
    SyncTask() = default;
    virtual ~SyncTask() = default;

    /**
     * @brief Run embedded scheduler to execute task in a desired frequency
     * @param[in] robotPtr robot's pointer
     * @param[in] spiDataPtr spi data's pointer
     * @param[in] spiDataListPtr spi data list's pointer
     * @param[in] robotDataPtr robot data's pointer
     * @param[in] robotDataListPtr robot data list's pointer
     * @param[in] logPtr robot's log pointer
     * @param[in] planName the name of the executing work plan
     * @return Status code
     */
    Status runScheduler(flexiv::Robot* robotPtr, SPIData* spiDataPtr,
        std::list<SPIData>* spiDataListPtr, RobotData* robotDataPtr,
        std::list<RobotData>* robotDataListPtr, flexiv::Log* logPtr,
        std::string planName);
};

} /* namespace kostal */

#endif /* FLEXIVRDK_SYNCTASK_HPP_ */