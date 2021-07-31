#pragma once

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "Gpio.h"
#include "SPI.h"

namespace Lora
{
    class Lora
    {
    private:
        spi_device_handle_t _spi_handle;
        Gpio::GpioOutput _reset{GPIO_NUM_4, true};
        Ispi::Spi _spiLora;

    public:
        esp_err_t Init();
        uint8_t ReadRegister(uint8_t reg_addr);
        esp_err_t WriteRegister(uint8_t reg_addr, uint8_t reg_data);
        esp_err_t TransmitString(const char *data_tx);

    }; // class Lora
} // namespace Lora