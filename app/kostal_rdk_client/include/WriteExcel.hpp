/**
 * @file WriteExcel.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */
#ifndef FLEXIVRDK_WRITEEXCEL_HPP_
#define FLEXIVRDK_WRITEEXCEL_HPP_

#include "KostalStates.hpp"
#include "KostalLog.hpp"

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
     * @brief According to spi timestamp,find the closest robot timestamp
     * @param[in] robotIterator iterator of RobotData list
     * @param[in] spiIterator iterator of SPIData list
     * @param[in] robotDataListPtr robot data list's pointer
     * @param[in] spiDataListPtr spi data list's pointer
     * @return iterator of RobotData list
     */
    std::list<RobotData>::iterator findRobotIterator(std::list<RobotData>::iterator robotIterator,
        std::list<SPIData>::iterator spiIterator, std::list<RobotData> *robotDataListPtr, std::list<SPIData> *spiDataListPtr);

    /**
     * @brief Write the global spi data list and robot data after synchronized.
     * @param[in] excelFile the csv type file to record robot and spi data 
     * @param[in] index the index of SPIData
     * @param[in] i_robotData robot data list's pointer
     * @param[in] i_spiData spi data list's pointer
     */
    void write2Excel(std::fstream &excelFile, int index, std::list<RobotData>::iterator i_robotData, std::list<SPIData>::iterator i_spiData);

public:
    int64_t firstTimestamp{0};
};

} /* namespace kostal */

#endif /* FLEXIVRDK_WRITEEXCEL_HPP_ */
