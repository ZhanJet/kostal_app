/**
 * @file WriteExcel.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */
#ifndef FLEXIVRDK_WRITEEXCEL_HPP_
#define FLEXIVRDK_WRITEEXCEL_HPP_

#include "KostalStates.hpp"

namespace kostal {

/**
 * @class WriteExcel
 * @brief Base class of writing different data into an excel file
 */
class WriteExcel
{
public:
    WriteExcel() = default;
    virtual ~WriteExcel() = default;

    /**
     * @brief Write the global spi data list and robot data list into a csv file
     * with associated name
     * @param[in] taskType the type of the task, can be NORMAL, BIAS, DUMMY
     * @param[in] taskName the name of the task
     * @param[in] robotDataListPtr robot data list's pointer
     * @param[in] spiDataListPtr spi data list's pointer
     * @param[in] logPtr robot's log pointer
     * @return Status code
     */
    Status writeDataToExcel(std::string taskType, std::string taskName,
        std::list<RobotData>* robotDataListPtr,
        std::list<SPIData>* spiDataListPtr, flexiv::Log* logPtr);

    /**
     * @brief Divide the global spi data list and robot data list into two
     * desired data list without some useless node data
     * @param[in] robotDataListPtr robot data list's pointer
     * @param[in] spiDataListPtr spi data list's pointer
     * @param[in] leftRobotDataListPtr left robot data list's pointer
     * @param[in] leftSPIDataListPtr left spi data list's pointer
     * @param[in] rightRobotDataListPtr right robot data list's pointer
     * @param[in] rightSPIDataListPtr right spi data list's pointer
     * @param[in] logPtr robot's log pointer
     * @return Status code
     */
    Status divideData(std::list<RobotData>* robotDataListPtr,
        std::list<SPIData>* spiDataListPtr,
        std::list<RobotData>* leftRobotDataListPtr,
        std::list<SPIData>* leftSPIDataListPtr,
        std::list<RobotData>* rightRobotDataListPtr,
        std::list<SPIData>* rightSPIDataListPtr, flexiv::Log* logPtr);

    /**
     * @brief Write spi data list and robot data list into csv file with given
     * lever side: Left or Right
     * @param[in] taskType the type of the task, can be NORMAL, BIAS, DUMMY
     * @param[in] taskName the name of the task
     * @param[in] leverSide the side of the lever to be written to excel
     * @param[in] leverRobotDataListPtr one lever's robot data list pointer
     * @param[in] leverSPIDataListPtr one lever's spi data list pointer
     * @param[in] logPtr robot's log pointer
     * @return Status code
     */
    Status writeLeverDataToExcel(std::string taskType, std::string taskName,
        std::string leverSide, std::list<RobotData>* leverRobotDataListPtr,
        std::list<SPIData>* leverSPIDataListPtr, flexiv::Log* logPtr);

    /**
     * @brief Write the global spi data list and robot data list into two csv
     * files with left and right
     * @param[in] taskType the type of the task, can be NORMAL, BIAS, DUMMY
     * @param[in] taskName the name of the task
     * @param[in] robotDataListPtr robot data list's pointer
     * @param[in] spiDataListPtr spi data list's pointer
     * @param[in] leftRobotDataListPtr left robot data list's pointer
     * @param[in] leftSPIDataListPtr left spi data list's pointer
     * @param[in] rightRobotDataListPtr right robot data list's pointer
     * @param[in] rightSPIDataListPtr right spi data list's pointer
     * @param[in] logPtr robot's log pointer
     * @return Status code
     */
    Status writeTwoExcel(std::string taskType, std::string taskName,
        std::list<RobotData>* robotDataListPtr,
        std::list<SPIData>* spiDataListPtr,
        std::list<RobotData>* leftRobotDataListPtr,
        std::list<SPIData>* leftSPIDataListPtr,
        std::list<RobotData>* rightRobotDataListPtr,
        std::list<SPIData>* rightSPIDataListPtr, flexiv::Log* logPtr);
};

} /* namespace kostal */

#endif /* FLEXIVRDK_WRITEEXCEL_HPP_ */
