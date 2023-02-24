/**
 * @file SPIOperations.cpp
 * @date Nov 29, 2022
 * @author Chenchen Liu
 */

#include "SPIOperations.hpp"
#include "ControlSPI.hpp"
#include <chrono>

namespace kostal {

Status SPIOperations::buildSPIConnectionSocket()
{
    flexiv::Log log;
    int ret;
    VSI_INIT_CONFIG SPI_Config;
    // Scan connected device
    ret = VSI_ScanDevice(1);
    if (ret <= 0) {
        log.error(
                    "The SPI device can not be found, please check the USB "
                    "interface!");
        return SPI;
    }
    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS) {
        log.error("The SPI device can not be open, please check SPI device!");
        return SPI;
    }
    SPI_Config.ControlMode = 0;
    SPI_Config.MasterMode = 0; // Slave Mode
    // Clock Polarity and Phase must be same as master
    SPI_Config.CPHA = g_CPHA;
    SPI_Config.CPOL = g_CPOL;
    SPI_Config.LSBFirst = g_LSB;
    SPI_Config.TranBits = 8; // Support 8bit mode only
    SPI_Config.SelPolarity = g_SELPOL;
    SPI_Config.ClockSpeed = 1395000;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS) {
        log.error("-The SPI device can not be initialized, please check SPI "
                    "device or use sudo!");
        return SPI;
    }
    return SUCCESS;
}

Status SPIOperations::reinitSPI()
{
    flexiv::Log log;
    int ret;
    VSI_INIT_CONFIG SPI_Config;

    ret = VSI_CloseDevice(VSI_USBSPI, 0);
    if (ret != ERR_SUCCESS)
    {
        log.error("The SPI device can not be close.");
        return SPI;
    }

    ret = VSI_ScanDevice(1);
    if (ret <= 0)
    {
        log.error("The SPI device can not be found, please check the USB interface!");
        return SPI;
    }

    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS) {
        log.error("The SPI device can not be open, please check SPI device!");
        return SPI;
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));
    SPI_Config.ControlMode = 0;
    SPI_Config.MasterMode = 0; // Slave Mode
    // Clock Polarity and Phase must be same as master
    SPI_Config.CPHA = g_CPHA;
    SPI_Config.CPOL = g_CPOL;
    SPI_Config.LSBFirst = g_LSB;
    SPI_Config.TranBits = 8; // Support 8bit mode only
    SPI_Config.SelPolarity = g_SELPOL;
    SPI_Config.ClockSpeed = 1395000;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    std::cout << ret << std::endl;
    if (ret != ERR_SUCCESS) {
        log.error("--The SPI device can not be initialized, please check SPI device or use sudo!");
        return SPI;
    }
    return SUCCESS;
}

Status SPIOperations::buildSPIConnection()
{
    flexiv::Log log;
    int ret;
    VSI_INIT_CONFIG SPI_Config;
    // Scan connected device
    ret = VSI_ScanDevice(0);
    if (ret <= 0) {
        log.error("The SPI device can not be found, please check the USB interface!");
        return SPI;
    }
    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS) {
        log.error("The SPI device can not be open, please check SPI device!");
        return SPI;
    }
    SPI_Config.ControlMode = 0;
    SPI_Config.MasterMode = 0; // Slave Mode
    SPI_Config.CPHA = 0; // Clock Polarity and Phase must be same as master
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8; // Support 8bit mode only
    SPI_Config.SelPolarity = 0;
    SPI_Config.ClockSpeed = 1395000;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS) {
        log.error("---The SPI device can not be initialized, please check SPI device!");
        return SPI;
    }
    return SUCCESS;
}

Status SPIOperations::collectSPIData(SPIData* spiDataPtr, std::list<SPIData>* spiDataListPtr, flexiv::Log* logPtr)
{
    // Global atomic switch
    while (g_collectSwitch) {
        uint8_t read_buffer[10240] = {0};
        int32_t read_data_num = 0;

        int ret = VSI_SlaveReadBytes(VSI_USBSPI, 0, read_buffer, &read_data_num, 10);

        if (ret != ERR_SUCCESS) {
            k_log->error("The SPI device read data error");
            return SPI;
        }
        // filter and only keep data with 16 bytes length
        if (read_data_num == 16) {
            for (int i = 0; i < read_data_num; i++) {
                spiDataPtr->SPISensor[i] = read_buffer[i];
                spiDataPtr->timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                spiDataListPtr->push_back(*spiDataPtr);
            }
        }
    }
    std::cout << "spi collect finish!" << std::endl;
    return SUCCESS;
}

} /* namespace kostal */
