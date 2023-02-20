/**
 * @file KostalStates.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */

#ifndef FLEXIVRDK_KOSTALSTATES_HPP_
#define FLEXIVRDK_KOSTALSTATES_HPP_

#include "SystemParams.hpp"

namespace kostal {

/**
 * @class RobotData
 * @brief The class of the robot data type
 */
class RobotData
{
public:
    RobotData() = default;
    virtual ~RobotData() = default;
    std::string nodeName;
    std::vector<double> tcpPose; // [7]
    std::vector<double> flangePose; // [7]
    std::vector<double> rawDataForceSensor; // [6]
    int64_t timestamp{0};

public:
    // copy constructor
    RobotData(const RobotData& r)
    {
        nodeName = r.nodeName;
        tcpPose = r.tcpPose;
        flangePose = r.flangePose;
        rawDataForceSensor = r.rawDataForceSensor;
        timestamp = r.timestamp;
    }
};

/**
 * @class SPIData
 * @brief The class of the spi data type
 */
class SPIData
{
public:
    SPIData() = default;
    virtual ~SPIData() = default;
    uint8_t SPISensor[16] = {0};
    int64_t timestamp{0};

public:
    // copy constructor
    SPIData(const SPIData& s)
    {
        for (int i = 0; i < 16; i++) {
            SPISensor[i] = s.SPISensor[i];
        }
        timestamp = s.timestamp;
    }

    // overload operator functions
//    SPIData& operator=(uint8_t* buffer)
//    {
//        memcpy(SPISensor, buffer, sizeof(uint8_t) * 16);
//        return *this;
//    }

    // clear SPISensor[16]
    void clearSensor() { SPISensor[16] = {0}; timestamp = 0;}
};

/**
 * @class KostalData
 * @brief The class of the kostal data type (Not used yet)
 * It is the combination of the robot data and spi data
 */
class KostalData
{
public:
    KostalData() = default;
    virtual ~KostalData() = default;
    std::string nodeName;
    std::vector<double> tcpPose; // [7]
    std::vector<double> flangePose; // [7]
    std::vector<double> rawDataForceSensor; // [6]
    uint8_t SPISensor[16] = {0};

public:
    // copy constructor
    KostalData(const KostalData& k)
    {
        nodeName = k.nodeName;
        tcpPose = k.tcpPose;
        flangePose = k.flangePose;
        rawDataForceSensor = k.rawDataForceSensor;
        for (int i = 0; i < 16; i++) {
            SPISensor[i] = k.SPISensor[i];
        }
    }
};

} /* namespace kostal */

#endif /*FLEXIVRDK_KOSTALSTATES_HPP_*/
