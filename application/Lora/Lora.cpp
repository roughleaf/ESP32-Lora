#include "Lora.h"

#define MODE_LONG_RANGE_MODE (1 << 7)

namespace LORA
{
    bool Lora::_irq_enabled {false};

    esp_err_t Lora::spiSetup(SPI::Spi* l_spi, const int ss, gpio_num_t reset_pin)
    {
        esp_err_t status{ESP_OK};

        _spi = l_spi;

        //_spi.init(SPI3_HOST, spi_3_miso, spi_3_mosi, spi_3_sclk);
        _reset.init(reset_pin, true);

        status |= _spi->registerDevice(0, ss);

        //_reset.off();

        return status;
    }

    // Pseudo code to initialise
    esp_err_t Lora::init(uint64_t frequency)
    {
        esp_err_t status { ESP_OK };

        _reset.on();
        vTaskDelay(pdMS_TO_TICKS(200));
        _reset.off();
        vTaskDelay(pdMS_TO_TICKS(200));

        status |= _writeRegister(RegOpMode, ModeSleep);               // Sleep mode
        status |= _writeRegister(RegOpMode, ModeLongRangeMode);       // Lora Mode
        status |= _setFrequency(frequency);

        _setInterruptTxRx(rx_int); // Initialise with RxDone interrupt enabled

        // Spreading Factor 6. Highest datarate for shortest time on air. See datasheet Page 27
        /*status |= _writeRegister(Lora_RegModemConfig2, 0x06 << 4 | 0x01 << 2);     // Set SpreadingFactor = 6, CRC on
        status |= _writeRegister(Lora_RegModemConfig1, (0x08 << 4) | (0x02 << 1) | 0x01);  // Set bandwidth to 250Khz, error code rate 4/6, Implicit header mode        
        status |= _writeRegister(Lora_RegDetectOptimize, 0b101); // Set the bit field DetectionOptimize of register RegLoRaDetectOptimize to value "0b101"        
        status |= _writeRegister(Lora_RegDetection_Threshold, 0x0C); // Write 0x0C in the register RegDetectionThreshold.    */    

        status |= _writeRegister(RegLna, _readRegister(RegLna) | 0x03);  // Enable LNA boost (HF)
        status |= _writeRegister(Lora_RegModemConfig3, 0x04);             // Set Auto AGC

        setTxPaLevel(17, true);

        // RegPayloadLength
        status |= _writeRegister(Lora_RegIrqFlags, 0xB7); // Only TxDone and RxDone IRQs enabled

        standBy();

        return status;
    }

    esp_err_t Lora::_setInterruptTxRx(lora_interrupt_t dio0)
    {
        esp_err_t status{ESP_OK};
        _dio0 = dio0;

        switch (dio0)
        {
        case rx_int:
            status = _writeRegister(RegDioMapping1, Lora_RxDone);
            break;
        case tx_int:
            status = _writeRegister(RegDioMapping1, Lora_TxDone);
            break;
        }

        return status;
    }

    esp_err_t Lora::irqSetup(gpio_num_t irq_pin, esp_event_handler_t lora_e_h)
    {
        esp_err_t status {ESP_OK};

        status = _lora_irq.init(irq_pin);
        status |= _lora_irq.enableInterrupt(irq_pin);
        status |= _lora_irq.setEventHandler(lora_e_h);

        return status;
    }

    void Lora::irqEnable(bool irq_enabled)
    {
        _irq_enabled = irq_enabled;
    }

    esp_err_t Lora::_setFrequency(uint64_t frequency)
    {
        esp_err_t status { ESP_OK };
        _frequency = frequency;

        uint64_t frf = (frequency << 19) / 32000000;

        status |= _writeRegister(RegFrfMsb, frf >> 16);
        status |= _writeRegister(RegFrfMid, frf >> 8);
        status |= _writeRegister(RegFrfLsb, frf >> 0);

        return status;
    }

    uint8_t Lora::_readRegister(uint8_t reg_addr)
    {        
        return _spi->readRegister(reg_addr);
    }

    esp_err_t Lora::_writeRegister(uint8_t reg_addr, uint8_t reg_data)
    {
        esp_err_t status = ESP_OK;

        status = _spi->writeRegister(reg_addr, reg_data);

        return status;
    }

    esp_err_t Lora::transmitString(const char *data_tx)
    {
        esp_err_t status{ESP_OK};
        // TODO Set FifoPtrAddr to FifoTxPtrBase.
        // TODO Write PayloadLength bytes to the FIFO (RegFifo)
        // TODO Set Mode to TX
        // TODO Check and handle TX done interrupt

        return status;
    }

    esp_err_t Lora::transmitByte(const char data_tx)
    {
        esp_err_t status{ESP_OK};
        _setInterruptTxRx(tx_int);
        status |= _writeRegister(RegOpMode, ModeLongRangeMode | ModeStandby);   // Set to Standby
        // Set FifoPtrAddr to FifoTxPtrBase.
        _writeRegister(Lora_RegFifoAddrPtr, 0x80);               // Tx FIFO start at 0x80
        _writeRegister(RegFifo, data_tx);                        // Write byte to TX FIFO
        _writeRegister(Lora_RegPayloadLength, 0x01);             // Payload Length is 1 byte
        status |= _writeRegister(RegOpMode, ModeLongRangeMode | ModeTransmit);   // Set to TX mode
        // TODO Check and handle TX done interrupt

        return status;
    }

    esp_err_t Lora::listen(void)
    {
        esp_err_t status{ESP_OK};
        _setInterruptTxRx(rx_int);
        status |= _writeRegister(RegOpMode, ModeLongRangeMode | ModeStandby);   // Set to Standby


        status |= _writeRegister(Lora_RegFifoAddrPtr, 0x00);               // Rx FIFO start at 0x00
        status |= _writeRegister(Lora_RegFifoRxBaseAddr, 0x00);
        status |= _writeRegister(Lora_RegPayloadLength, 0x01);             // Payload Length is 1 byte
        //_writeRegister(Lora_RegRxNbBytes, 0x01);             // Payload Length is 1 byte
        status |= _writeRegister(RegOpMode, ModeLongRangeMode | ModeReceiveContinuous);   // Set to RX Continuous mode
        // TODO Check and handle RX done interrupt

        return status;
    }

    esp_err_t Lora::setTxPaLevel(uint8_t pa_level, bool pa_boost)
    {
        esp_err_t status{ESP_OK};

        // RFO_pin used
        if(!pa_boost)
        {
            if(pa_level > 14) { pa_level = 14; }

            status |= _writeRegister(RegPaConfig, 0x70 | pa_level);
        }
        // PA Boost_pin used
        else
        {
            // High power application not implimented.
            // Attempts to enable dB of 20 will default to 17
            if(pa_level > 17) { pa_level = 17; }
            if(pa_level < 2) { pa_level = 2; }

            status |= _writeRegister(RegPaDac, 0x84);
            status |= _writeRegister(RegPaConfig, PaBoostPin | (pa_level - 2));
            status |= setOCP(100);
        }

        return status;
    }

    esp_err_t Lora::setOCP(uint8_t current)
    {
        esp_err_t status{ESP_OK};

        uint8_t trim = 11;

        if(current <= 120)
        {
            trim = (current - 45) / 5;
        }
        else
        {
            if(current > 240) { current = 240; }
            trim = (current + 30) / 10;
        }

        return status = _writeRegister(RegOcp, 0x20 | (0x1F & trim));
    }

    uint8_t Lora::readReceivedByte(void)
    {
        return(_readRegister(RegFifo));
    }

    esp_err_t Lora::standBy(void)
    {
        esp_err_t status{ESP_OK};
        status |= _writeRegister(RegOpMode, ModeLongRangeMode | ModeStandby);   // Set to Standby

        return status;
    }

    esp_err_t Lora::sleep(void)
    {
        esp_err_t status{ESP_OK};
        status |= _writeRegister(RegOpMode, ModeLongRangeMode | ModeSleep);   // Set to Standby

        return status;
    }

    esp_err_t Lora::clearIrqFlags()
    {
        esp_err_t status{ESP_OK};

        status |= _writeRegister(Lora_RegFifoAddrPtr, 0x00);                 // Reset FIFO pointer to 0
        status |= _writeRegister(Lora_RegIrqFlags, IrqTxDone | IrqRxDone);   // Clear IRQ flags

        return status;
    }

    spi_device_handle_t Lora::getSpiHandle(void)
    {
        return _spi->getHandle();
    }

    Lora::lora_interrupt_t Lora::getInterruptMode(void)
    {
        return _dio0;
    }

    int Lora::getRSSI(void)
    {
        return static_cast<int>(_readRegister(Lora_RegRssiValue)) - 164;
    }

    int Lora::getPacketRSSI(void)
    {
        return static_cast<int>(_readRegister(Lora_RegPktRssiValue)) - 164;
    }

} // namespace Lora