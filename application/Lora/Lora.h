#pragma once

#include "driver/spi_common.h"
#include "driver/spi_master.h"

namespace Lora
{
    class Lora
    {
    private:
        spi_device_handle_t _spi_handle;

    public:
        esp_err_t Init(void);
        uint8_t ReadRegister(uint8_t reg_addr);
        esp_err_t WriteRegister(uint8_t reg_addr, uint8_t reg_data);
        esp_err_t TransmitString(const char *data_tx);

    }; // class Lora
} // namespace Lora