#include "Lora.h"

namespace LORA
{
    esp_err_t Lora::Init(SPI::Spi* l_spi)
    {
        esp_err_t status{ESP_OK};

        _spi = l_spi;

        //status |= _spi.Init(SPI3_HOST, 19, 23, 18);
        status |= _spi->RegisterDevice(0, 5);

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