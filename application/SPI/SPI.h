#pragma once

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "Gpio.h"
namespace SPI
{
    class Spi
    {
    public:
        esp_err_t Init(const spi_host_device_t spi_peripheral, const int pin_miso, const int pin_mosi, const int pin_sclk);
        esp_err_t RegisterDevice(const uint8_t mode, const int ss);
        uint8_t ReadRegister(uint8_t reg_addr);
        esp_err_t WriteRegister(uint8_t reg_addr, uint8_t reg_data);

    private:
        spi_bus_config_t spi_bus_cfg;
        spi_device_interface_config_t spi_interface_cfg;

        spi_host_device_t _spi_peripheral{};       
        spi_device_handle_t _handle{};
        uint8_t _ss;
        spi_transaction_t spi_transaction;

        esp_err_t _transfer_byte(uint8_t reg_addr, uint8_t data);
        esp_err_t _transfer_multiples_bytes(uint8_t reg_addr, size_t data_length);

    }; // class spi
} // namespace Ispi