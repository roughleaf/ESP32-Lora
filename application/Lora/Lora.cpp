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

        _reset.Off();

        return status;
    }

    // Pseudo code to initialise
    int Lora::Init()
    {
        WriteRegister(RegOpMode, 0x00);         // Sleep mode
        WriteRegister(RegOpMode, 0x01 << 7);    // Lora Mode

        // Spreading Factor 6. Highest datarate for shortest time on air. See datasheet Page 27
        WriteRegister(Lora_RegModemConfig2, 0x06 << 4);     // Set SpreadingFactor = 6
        WriteRegister(Lora_RegModemConfig1, (0x08 << 4) | (0x02 << 1) | 0x01);  // Set bandwidth to 250Khz, error code rate 4/6, Implicit header mode        
        WriteRegister(Lora_RegDetectOptimize, 0b101); // Set the bit field DetectionOptimize of register RegLoRaDetectOptimize to value "0b101"        
        WriteRegister(Lora_RegDetection_Threshold, 0x0C); // Write 0x0C in the register RegDetectionThreshold.
        return 0;
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