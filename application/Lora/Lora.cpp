#include "Lora.h"

namespace LORA
{
    bool Lora::_irq_enabled {false};

    esp_err_t Lora::SpiSetup(SPI::Spi* l_spi, const int ss, gpio_num_t reset_pin)
    {
        esp_err_t status{ESP_OK};

        _spi = l_spi;

        //_spi.Init(SPI3_HOST, spi_3_miso, spi_3_mosi, spi_3_sclk);
        _reset.Init(reset_pin, true);

        status |= _spi->RegisterDevice(0, ss);

        _reset.Off();

        return status;
    }

    // Pseudo code to initialise
    esp_err_t Lora::Init()
    {
        esp_err_t status { ESP_OK };

        status |= WriteRegister(RegOpMode, 0x00);         // Sleep mode
        status |= WriteRegister(RegOpMode, 0x01 << 7);    // Lora Mode

        _setInterruptTxRx(rx_int); // Initialise with RxDone interrupt enabled

        // Spreading Factor 6. Highest datarate for shortest time on air. See datasheet Page 27
        status |= WriteRegister(Lora_RegModemConfig2, 0x06 << 4 | 0x01 << 2);     // Set SpreadingFactor = 6, CRC on
        status |= WriteRegister(Lora_RegModemConfig1, (0x08 << 4) | (0x02 << 1) | 0x01);  // Set bandwidth to 250Khz, error code rate 4/6, Implicit header mode        
        status |= WriteRegister(Lora_RegDetectOptimize, 0b101); // Set the bit field DetectionOptimize of register RegLoRaDetectOptimize to value "0b101"        
        status |= WriteRegister(Lora_RegDetection_Threshold, 0x0C); // Write 0x0C in the register RegDetectionThreshold.

        status |= SetFrequency(433000000); // Set to 433Mhz

        // RegPayloadLength
        status |= WriteRegister(Lora_RegIrqFlags, 0xB7); // Only TxDone and RxDone IRQs enabled
        // RegIrqFlags
        return status;
    }

    esp_err_t Lora::_setInterruptTxRx(lora_interrupt_t dio0)
    {
        esp_err_t status{ESP_OK};
        _dio0 = dio0;

        switch (dio0)
        {
        case rx_int:
            status = WriteRegister(RegDioMapping1, Lora_RxDone);
            break;
        case tx_int:
            status = WriteRegister(RegDioMapping1, Lora_TxDone);
            break;
        }

        return status;
    }

    esp_err_t Lora::IrqSetup(gpio_num_t irq_pin, esp_event_handler_t lora_e_h)
    {
        esp_err_t status {ESP_OK};

        status = _lora_irq.Init(irq_pin);
        status |= _lora_irq.EnableInterrupt(irq_pin);
        status |= _lora_irq.SetEventHandler(lora_e_h);

        return status;
    }

    void Lora::IrqEnable(bool irq_enabled)
    {
        _irq_enabled = irq_enabled;
    }

    esp_err_t Lora::SetFrequency(uint64_t frequency)
    {
        esp_err_t status { ESP_OK };
        _frequency = frequency;

        uint64_t frf = (frequency << 19) / 32000000;

        status |= WriteRegister(RegFrfMsb, frf >> 16);
        status |= WriteRegister(RegFrfMid, frf >> 8);
        status |= WriteRegister(RegFrfLsb, frf >> 0);

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
        // TODO Set FifoPtrAddr to FifoTxPtrBase.
        // TODO Write PayloadLength bytes to the FIFO (RegFifo)
        // TODO Set Mode to TX
        // TODO Check and handle TX done interrupt

        return status;
    }

    esp_err_t Lora::TransmitByte(const char data_tx)
    {
        esp_err_t status{ESP_OK};
        _setInterruptTxRx(tx_int);
        status |= WriteRegister(RegOpMode, 0x01 << 7 | 0x01);   // Set to Standby
        // Set FifoPtrAddr to FifoTxPtrBase.
        WriteRegister(Lora_RegFifoAddrPtr, 0x80);               // Tx FIFO start at 0x80
        WriteRegister(RegFifo, data_tx);                        // Write byte to TX FIFO
        WriteRegister(Lora_RegPayloadLength, 0x01);             // Payload Length is 1 byte
        status |= WriteRegister(RegOpMode, 0x01 << 7 | 0x03);   // Set to TX mode
        // TODO Check and handle TX done interrupt

        return status;
    }

    esp_err_t Lora::ClearIrqFlags()
    {
        return WriteRegister(LORA::Lora_RegIrqFlags, 0x01 << 3 | 0x01 << 6);
    }

    spi_device_handle_t Lora::GetSpiHandle(void)
    {
        return _spi->GetHandle();
    }

    Lora::lora_interrupt_t Lora::GetInterruptMode(void)
    {
        return _dio0;
    }

} // namespace Lora