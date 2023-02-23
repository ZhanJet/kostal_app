/**
 * @file WriteExcel.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "WriteExcel.hpp"

namespace kostal {

// get current time in time_t format
std::string getTime()
{
    time_t timep;
    time(&timep); // get current time in time_t type
    char strTime[64];
    strftime(strTime, sizeof(strTime), "_%Y-%m-%d_%H:%M:%S", localtime(&timep));
    return strTime;
}

// transfer a double array of tcp quaternion to an array of euler
std::array<double, 3> quaternionToEuler(double* tcpPose)
{
    double M_Pi;
    double rad = 180 / M_PI;
    Eigen::Quaternion<double> q;
    std::array<double, 3> eulerAngle;
    q.w() = tcpPose[0];
    q.x() = tcpPose[1];
    q.y() = tcpPose[2];
    q.z() = tcpPose[3];
    auto euler = q.toRotationMatrix().eulerAngles(2, 1, 0);
    eulerAngle[0] = euler[2];
    eulerAngle[1] = euler[1];
    eulerAngle[2] = euler[0];
    return eulerAngle;
}

Status WriteExcel::writeDataToExcel(std::string taskType, std::string taskName,
    std::list<RobotData>* robotDataListPtr, std::list<SPIData>* spiDataListPtr,
    flexiv::Log* logPtr)
{
    if (spiDataListPtr->size() == 0) {
        logPtr->error("The collected spi data list is null, exiting...");
        return CSV;
    }

    if (robotDataListPtr->size() == 0) {
        logPtr->error("The collected robot data list is null, exiting...");
        return CSV;
    }

    std::fstream excelFile;
    std::string filePath = UPLOADADDRESS;
    std::string excelFileName = filePath + taskType + "/" + taskName + getTime() + ".csv";
    std::cout << "The generated file path is: " << excelFileName << std::endl;
    excelFile.open(excelFileName, std::ios::out);
    if (!excelFile.is_open()) {
        logPtr->error("The associated excel file is not created correctly");
        return CSV;
    }

    // Header
    excelFile << "NodeName"
              << ","
              << "Index"
              << ",";
    excelFile << "TCP_x"
              << ","
              << "TCP_y"
              << ","
              << "TCP_z"
              << ",";
    excelFile << "TCP_Rx"
              << ","
              << "TCP_Ry"
              << ","
              << "TCP_Rz"
              << ",";
    excelFile << "FLANGE_x"
              << ","
              << "FLANGE_y"
              << ","
              << "FLANGE_z"
              << ",";
    excelFile << "FLANGE_Rx"
              << ","
              << "FLANGE_Ry"
              << ","
              << "FLANGR_Rz"
              << ",";
    excelFile << "RawDataSensor0"
              << ","
              << "RawDataSensor1"
              << ","
              << "RawDataSensor2"
              << ",";
    excelFile << "RawDataSensor3"
              << ","
              << "RawDataSensor4"
              << ","
              << "RawDataSensor5"
              << ",";
    excelFile << "SPI0-0"
              << ","
              << "SPI0-1"
              << ","
              << "SPI0-2"
              << ","
              << "SPI0-3"
              << ","
              << "SPI0-4"
              << ",";
    excelFile << "SPI0-5"
              << ","
              << "SPI0-6"
              << ","
              << "SPI0-7"
              << ",";
    excelFile << "SPI1-0"
              << ","
              << "SPI1-1"
              << ","
              << "SPI1-2"
              << ","
              << "SPI1-3"
              << ","
              << "SPI1-4"
              << ",";
    excelFile << "SPI1-5"
              << ","
              << "SPI1-6"
              << ","
              << "SPI1-7"
              << ","
              << "timestamp"
              << std::endl;

    std::list<RobotData>::iterator i_robotData;
    std::list<SPIData>::iterator i_spiData = spiDataListPtr->begin();

    firstTimestamp = (*i_spiData).timestamp;

    int64_t spiTimestamp = (*i_spiData).timestamp;
    std::list<SPIData>::iterator i_spiDataSecond = std::next(i_spiData, 1);
    int64_t nextSpiTimestamp = (*i_spiDataSecond).timestamp;

    int index = 0;

    for(i_robotData = robotDataListPtr->begin(); i_robotData != robotDataListPtr->end(); i_robotData++)
    {
        int64_t robotTimestamp = (*i_robotData).timestamp;

        std::list<SPIData>::iterator i_spiDataNext = i_spiData;
        i_spiDataNext++;

        if(i_spiDataNext != spiDataListPtr->end())
        {
            if(nextSpiTimestamp < robotTimestamp)
            {
                write2Excel(excelFile, index, i_robotData, i_spiDataNext);

                i_spiData++;
                spiTimestamp = (*i_spiData).timestamp;

                std::list<SPIData>::iterator i_spiDataNext2 = i_spiData;
                i_spiDataNext2++;

                if(i_spiDataNext2 != spiDataListPtr->end())
                {
                    nextSpiTimestamp = (*i_spiDataNext2).timestamp;
                }
            }
            else
            {
                write2Excel(excelFile, index, i_robotData, i_spiData);
            }
        }
        else
        {
            write2Excel(excelFile, index, i_robotData, i_spiData);
        }

        index++;
    }

    excelFile.close();
    return SUCCESS;
}

void WriteExcel::write2Excel(std::fstream &excelFile, int index, std::list<RobotData>::iterator i_robotData, std::list<SPIData>::iterator i_spiData)
{
    // node name
    excelFile << (*i_robotData).nodeName << ",";

    // Index increase
    excelFile << std::to_string(index) << ",";

    // tcp xyz
    excelFile << (*i_robotData).tcpPose[0] << ",";
    excelFile << (*i_robotData).tcpPose[1] << ",";
    excelFile << (*i_robotData).tcpPose[2] << ",";

    // tcp euler data
    double quaternionTcp[4] = {(*i_robotData).tcpPose[3], (*i_robotData).tcpPose[4], (*i_robotData).tcpPose[5], (*i_robotData).tcpPose[6]};
    auto eulerTcp = quaternionToEuler(quaternionTcp);
    excelFile << eulerTcp[0] << ",";
    excelFile << eulerTcp[1] << ",";
    excelFile << eulerTcp[2] << ",";

    // flange xyz
    excelFile << (*i_robotData).flangePose[0] << ",";
    excelFile << (*i_robotData).flangePose[1] << ",";
    excelFile << (*i_robotData).flangePose[2] << ",";

    // flange euler data
    double quaternionFlange[4] = {(*i_robotData).flangePose[3], (*i_robotData).flangePose[4], (*i_robotData).flangePose[5], (*i_robotData).flangePose[6]};
    auto eulerFlange = quaternionToEuler(quaternionFlange);
    excelFile << eulerFlange[0] << ",";
    excelFile << eulerFlange[1] << ",";
    excelFile << eulerFlange[2] << ",";

    // raw sensor data
    for (int i = 0; i < 6; i++)
    {
        excelFile << (*i_robotData).rawDataForceSensor[i] << ",";
    }

    // spi sensor data
    for (int j = 0; j < 16; j++)
    {
        excelFile << std::setfill('0') << std::setw(2) << std::right << std::hex;
        excelFile << +static_cast<uint8_t>((*i_spiData).SPISensor[j]) << ",";
    }

    excelFile << std::to_string((*i_robotData).timestamp - firstTimestamp);

    excelFile << std::endl;
}

Status WriteExcel::divideData(std::list<RobotData>* robotDataListPtr,
    std::list<SPIData>* spiDataListPtr,
    std::list<RobotData>* leftRobotDataListPtr,
    std::list<SPIData>* leftSPIDataListPtr,
    std::list<RobotData>* rightRobotDataListPtr,
    std::list<SPIData>* rightSPIDataListPtr, flexiv::Log* logPtr)
{
    if (spiDataListPtr->size() == 0) {
        logPtr->error("The collected spi data list is null, exiting...");
        return CSV;
    }

    if (robotDataListPtr->size() == 0) {
        logPtr->error("The collected robot data list is null, exiting...");
        return CSV;
    }
    if (spiDataListPtr->size() != robotDataListPtr->size()) {
        logPtr->error(
            "The size of collected robot data and spi data is not same, "
            "exiting...");
        return CSV;
    }
    while (robotDataListPtr->size() > 0) {
        RobotData robotDataRow = robotDataListPtr->front();
        SPIData spiDataRow = spiDataListPtr->front();
        if (robotDataListPtr->begin()->nodeName == "Start") {
            robotDataListPtr->pop_front();
            spiDataListPtr->pop_front();
            continue;
        } else if (robotDataListPtr->begin()->nodeName.substr(0, 6)
                   == "LEVER1") {
            leftRobotDataListPtr->push_back(robotDataRow);
            leftSPIDataListPtr->push_back(spiDataRow);
        } else if (robotDataListPtr->begin()->nodeName.substr(0, 6)
                   == "LEVER2") {
            rightRobotDataListPtr->push_back(robotDataRow);
            rightSPIDataListPtr->push_back(spiDataRow);
        }
        robotDataListPtr->pop_front();
        spiDataListPtr->pop_front();
    }
    logPtr->info("The left and right data has been divided successfully!");
    return SUCCESS;
}

Status WriteExcel::writeLeverDataToExcel(std::string taskType,
    std::string taskName, std::string leverSide,
    std::list<RobotData>* leverRobotDataListPtr,
    std::list<SPIData>* leverSPIDataListPtr, flexiv::Log* logPtr)
{
    // Create excel file in assigned address
    std::fstream excelFile;
    std::string filePath = UPLOADADDRESS;
    std::string excelFileName = filePath + taskType + "/" + taskName + getTime() + leverSide + ".csv";
    std::cout << "The generated file path is: " << excelFileName << std::endl;
    excelFile.open(excelFileName, std::ios::out);
    if (!excelFile.is_open()) {
        logPtr->error("The associated " + leverSide + " excel file is not created correctly");
        return CSV;
    }

    // Header
    excelFile << "NodeName"
              << ","
              << "Index"
              << ",";
    excelFile << "TCP_x"
              << ","
              << "TCP_y"
              << ","
              << "TCP_z"
              << ",";
    excelFile << "TCP_Rx"
              << ","
              << "TCP_Ry"
              << ","
              << "TCP_Rz"
              << ",";
    excelFile << "FLANGE_x"
              << ","
              << "FLANGE_y"
              << ","
              << "FLANGE_z"
              << ",";
    excelFile << "FLANGE_Rx"
              << ","
              << "FLANGE_Ry"
              << ","
              << "FLANGR_Rz"
              << ",";
    excelFile << "RawDataSensor0"
              << ","
              << "RawDataSensor1"
              << ","
              << "RawDataSensor2"
              << ",";
    excelFile << "RawDataSensor3"
              << ","
              << "RawDataSensor4"
              << ","
              << "RawDataSensor5"
              << ",";
    excelFile << "SPI0-0"
              << ","
              << "SPI0-1"
              << ","
              << "SPI0-2"
              << ","
              << "SPI0-3"
              << ","
              << "SPI0-4"
              << ",";
    excelFile << "SPI0-5"
              << ","
              << "SPI0-6"
              << ","
              << "SPI0-7"
              << ",";
    excelFile << "SPI1-0"
              << ","
              << "SPI1-1"
              << ","
              << "SPI1-2"
              << ","
              << "SPI1-3"
              << ","
              << "SPI1-4"
              << ",";
    excelFile << "SPI1-5"
              << ","
              << "SPI1-6"
              << ","
              << "SPI1-7"
              << "," << std::endl;

    long index = 0;
    // Start iterating
    while (leverRobotDataListPtr->size() > 0) {
        RobotData robotDataRow = leverRobotDataListPtr->front();
        SPIData spiDataRow = leverSPIDataListPtr->front();

        // node name
        excelFile << robotDataRow.nodeName << ",";

        // Index increase
        excelFile << std::to_string(index) << ",";

        // tcp xyz
        excelFile << robotDataRow.tcpPose[0] << ",";
        excelFile << robotDataRow.tcpPose[1] << ",";
        excelFile << robotDataRow.tcpPose[2] << ",";

        // tcp euler data
        double quaternionTcp[4]
            = {robotDataRow.tcpPose[3], robotDataRow.tcpPose[4],
                robotDataRow.tcpPose[5], robotDataRow.tcpPose[6]};
        auto eulerTcp = quaternionToEuler(quaternionTcp);
        excelFile << eulerTcp[0] << ",";
        excelFile << eulerTcp[1] << ",";
        excelFile << eulerTcp[2] << ",";

        // flange xyz
        excelFile << robotDataRow.flangePose[0] << ",";
        excelFile << robotDataRow.flangePose[1] << ",";
        excelFile << robotDataRow.flangePose[2] << ",";

        // flange euler data
        double quaternionFlange[4]
            = {robotDataRow.flangePose[3], robotDataRow.flangePose[4],
                robotDataRow.flangePose[5], robotDataRow.flangePose[6]};
        auto eulerFlange = quaternionToEuler(quaternionFlange);
        excelFile << eulerFlange[0] << ",";
        excelFile << eulerFlange[1] << ",";
        excelFile << eulerFlange[2] << ",";

        // raw sensor data
        for (int i = 0; i < 6; i++) {
            excelFile << robotDataRow.rawDataForceSensor[i] << ",";
        }

        // spi sensor data
        for (int i = 0; i < 16; i++) {
            excelFile << std::setfill('0') << std::setw(2) << std::right
                      << std::hex;
            excelFile << +static_cast<uint8_t>(spiDataRow.SPISensor[i]) << ",";
        }

        // finish this line
        excelFile << std::endl;
        index++;
        leverRobotDataListPtr->pop_front();
        leverSPIDataListPtr->pop_front();
    }

    excelFile.close();
    return SUCCESS;
}

Status WriteExcel::writeTwoExcel(std::string taskType, std::string taskName,
    std::list<RobotData>* robotDataListPtr, std::list<SPIData>* spiDataListPtr,
    std::list<RobotData>* leftRobotDataListPtr,
    std::list<SPIData>* leftSPIDataListPtr,
    std::list<RobotData>* rightRobotDataListPtr,
    std::list<SPIData>* rightSPIDataListPtr, flexiv::Log* logPtr)
{
    Status result;
    result = divideData(robotDataListPtr, spiDataListPtr, leftRobotDataListPtr,
        leftSPIDataListPtr, rightRobotDataListPtr, rightSPIDataListPtr, logPtr);
    if (result != SUCCESS) {
        logPtr->error("The excel writing of spi and robot data is failed");
        return result;
    }
    // Check size is equal and not equal to 0
    if (leftSPIDataListPtr->size() == 0 || rightSPIDataListPtr->size() == 0) {
        logPtr->error(
            "The collected left or right spi data list is null, "
            "exiting...");
        return CSV;
    }
    if (leftRobotDataListPtr->size() == 0
        || rightRobotDataListPtr->size() == 0) {
        logPtr->error(
            "The collected left or right robot data list is null, "
            "exiting...");
        return CSV;
    }
    if (leftSPIDataListPtr->size() != leftRobotDataListPtr->size()) {
        logPtr->error(
            "The size of collected left robot data and spi data is not "
            "same, exiting...");
        return CSV;
    }
    if (rightSPIDataListPtr->size() != rightRobotDataListPtr->size()) {
        logPtr->error(
            "The size of collected right robot data and spi data is not "
            "same, exiting...");
        return CSV;
    }
    // write left lever data to excel
    std::string leverSide = "_Left";
    result = writeLeverDataToExcel(taskType, taskName, leverSide,
        leftRobotDataListPtr, rightSPIDataListPtr, logPtr);
    if (result != SUCCESS) {
        logPtr->error("The writing of left lever data to excel is failed");
        return result;
    }

    // write right lever data to excel
    leverSide = "_Right";
    result = writeLeverDataToExcel(taskType, taskName, leverSide,
        leftRobotDataListPtr, rightSPIDataListPtr, logPtr);
    if (result != SUCCESS) {
        logPtr->error("The writing of right lever data to excel is failed");
        return result;
    }

    logPtr->info(
        "Both of left and right lever excel files are generated "
        "successfully");
    return SUCCESS;
}

} /* namespace kostal */
