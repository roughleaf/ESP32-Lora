#include "Lora.h"

namespace LORA
{
    esp_err_t Lora::SpiSetup(SPI::Spi* l_spi, const int ss, gpio_num_t reset_pin)
    {
        esp_err_t status{ESP_OK};

        _spi = l_spi;
        _reset.Init(reset_pin, true);

        //status |= _spi.Init(SPI3_HOST, 19, 23, 18);
        status |= _spi->RegisterDevice(0, ss);

        _reset.Inactive();

        return status;
    }

    uint8_t Lora::ReadRegister(uint8_t reg_addr)
    {        
        return _spi->ReadRegister(reg_addr);
    }

    esp_err_t Lora::WriteRegister(uint8_t reg_addr, uint8_t reg_data)
    {
        esp_err_t status = ESP_OK;

        status = _spi->WriteRegister(reg_addr, reg_data);

        return status;
    }

    esp_err_t Lora::TransmitString(const char *data_tx)
    {
        esp_err_t status{ESP_OK};

        return status;
    }

} // namespace Lora