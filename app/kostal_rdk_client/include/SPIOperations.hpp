/**
 * @file SPIOperations.hpp
 * @copyright Copyright (C) 2016-2022 Flexiv Ltd. All Rights Reserved.
 */

#ifndef FLEXIVRDK_SPIOPERATIONS_HPP_
#define FLEXIVRDK_SPIOPERATIONS_HPP_

#include "KostalStates.hpp"

namespace kostal {

/**
 * @class SPIOperations
 * @brief Base class for SPI device operations like connect and collect
 */
class SPIOperations
{
public:
    SPIOperations() = default;
    virtual ~SPIOperations() = default;

    /**
     * @brief Build connection between SPI device and system with socket message
     * @return Status code
     */
    Status buildSPIConnectionSocket();

    /**
     * @brief Build connection between SPI device and system with socket message
     * @return Status code
     */
    Status reinitSPI();

    /**
     * @brief Build the connection to SPI device with default params
     * @return Status code
     */
    Status buildSPIConnection();

    /**
     * @brief Read SPI data from the USB-SPI device, put them into the SPI data
     * list
     * @param[in] logPtr robot's log pointer
     * @param[in] spiDataPtr spi data's pointer
     * @return Status code
     */
    Status collectSPIData(SPIData* spiDataPtr, flexiv::Log* logPtr);
};

} /* namespace kostal */

#endif /* FLEXIVRDK_SPIOPERATIONS_HPP_ */