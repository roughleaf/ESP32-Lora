#include "Lora.h"

namespace Lora
{
    esp_err_t Lora::Init()
    {
        esp_err_t status{ESP_OK};

        status |= _spiLora.Init(SPI3_HOST, 19, 23, 18);
        status |= _spiLora.RegisterDevice(0, 5);

        _reset.Inactive();

        return status;
    }

    uint8_t Lora::ReadRegister(uint8_t reg_addr)
    {        
        return _spiLora.ReadRegister(reg_addr);
    }

    esp_err_t Lora::WriteRegister(uint8_t reg_addr, uint8_t reg_data)
    {
        esp_err_t status = ESP_OK;

        status = _spiLora.WriteRegister(reg_addr, reg_data);

        return status;
    }

    esp_err_t Lora::TransmitString(const char *data_tx)
    {
        esp_err_t status{ESP_OK};

        return status;
    }

} // namespace Lora