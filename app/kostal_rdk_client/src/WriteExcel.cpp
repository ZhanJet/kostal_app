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
    if(!isBiasMode){
        if (spiDataListPtr->size() == 0) {
        k_log->error("The collected spi data list is null, exiting...");
        return CSV;
    }
    }
    

    if (robotDataListPtr->size() == 0) {
        k_log->error("The collected robot data list is null, exiting...");
        return CSV;
    }

    std::fstream excelFile;
    std::string filePath = UPLOADADDRESS;
    std::string excelFileName = filePath + taskType + "/" + taskName + getTime() + ".csv";
    std::cout << "The generated file path is: " << excelFileName << std::endl;
    excelFile.open(excelFileName, std::ios::out);
    if (!excelFile.is_open()) {
        k_log->error("The associated excel file is not created correctly");
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

    std::list<SPIData>::iterator spiIterator = spiDataListPtr->begin();
    std::list<RobotData>::iterator robotIterator = robotDataListPtr->begin();
    if(isBiasMode)
    {
        for(int index = 0; index < robotDataListPtr->size(); index++)
        {
            write2Excel(excelFile, index, robotIterator, spiIterator);            
            robotIterator++;
        }

    }else{
        firstTimestamp = (*spiIterator).timestamp;

        for(int index = 0; spiIterator != spiDataListPtr->end(); spiIterator++)
        {   
            robotIterator = findRobotIterator(robotIterator, spiIterator, robotDataListPtr, spiDataListPtr);
            write2Excel(excelFile, index, robotIterator, spiIterator);
            index++;
        }

    }
    

    excelFile.close();
    return SUCCESS;
}

std::list<RobotData>::iterator WriteExcel::findRobotIterator(std::list<RobotData>::iterator robotIterator, std::list<SPIData>::iterator spiIterator, std::list<RobotData>* robotDataListPtr, std::list<SPIData>* spiDataListPtr)
{
    std::list<RobotData>::iterator nextRobotIterator = robotIterator;
    while((*nextRobotIterator).timestamp < (*spiIterator).timestamp)
    {
        if(std::next(nextRobotIterator) == robotDataListPtr->end() || nextRobotIterator == robotDataListPtr->end())
        {
            break;
        }
        else
        {
            nextRobotIterator++;
        }
    }
    std::list<RobotData>::iterator lastRobotIterator = robotIterator;
    if(nextRobotIterator != robotDataListPtr->begin())
    {
        lastRobotIterator = std::prev(nextRobotIterator);
    }
    int64_t lastValue = (*spiIterator).timestamp - (*lastRobotIterator).timestamp;
    int64_t nextValue = (*nextRobotIterator).timestamp - (*spiIterator).timestamp;
    if(lastValue < nextValue)
    {
        return lastRobotIterator;
    }
    else
    {
        return nextRobotIterator;
    }
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
    if(isBiasMode)
        excelFile << std::to_string((*i_robotData).timestamp);
    else    
        excelFile << std::to_string((*i_spiData).timestamp - firstTimestamp);    
    
    excelFile << std::endl;
}

} /* namespace kostal */
