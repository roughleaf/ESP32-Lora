#pragma once

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "Gpio.h"
namespace SPI
{
    class Spi
    {
    public:
        esp_err_t init(const spi_host_device_t spi_peripheral, const int pin_miso, const int pin_mosi, const int pin_sclk);
        esp_err_t registerDevice(const uint8_t mode, const int ss);
        uint8_t readRegister(uint8_t reg_addr);
        esp_err_t writeRegister(uint8_t reg_addr, uint8_t reg_data);
        spi_device_handle_t getHandle(void);
        esp_err_t writeRegisterMultipleBytes(uint8_t reg_addr, uint8_t* reg_data_buffer, uint8_t byteCount);
        esp_err_t readRegisterMultipleBytes(uint8_t reg_addr, uint8_t* reg_data_buffer, uint8_t byteCount);

    private:
        spi_bus_config_t spi_bus_cfg;
        spi_device_interface_config_t spi_interface_cfg;
        spi_device_handle_t _handle{};
        spi_host_device_t _spi_peripheral{};       
        
        uint8_t _ss;
        spi_transaction_t spi_transaction;

        esp_err_t _transfer_byte(uint8_t reg_addr, uint8_t data);
        esp_err_t _transfer_multiples_bytes(uint8_t reg_addr, uint8_t* tx_buf, uint8_t* rx_buf, size_t data_length);

    }; // class spi
} // namespace SPI