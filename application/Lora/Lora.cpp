#include "Lora.h"

#define MODE_LONG_RANGE_MODE (1 << 7)

namespace LORA
{
    bool Lora::_irq_enabled {false};

    esp_err_t Lora::spiSetup(SPI::Spi* l_spi, const int ss, gpio_num_t reset_pin)
    {
        esp_err_t status{ESP_OK};

        _spi = l_spi;

        status |= _reset.init(reset_pin, true);

        status |= _spi->registerDevice(0, ss);

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

        sleep();
        status |= _writeRegister(RegOpMode, ModeLongRangeMode);       // Lora Mode
        status |= _setFrequency(frequency);

        _setInterruptTxRx(rx_int); // Initialise with RxDone interrupt enabled

        setSpreadingFactor(7);      // Set default and force AutomaticIFOn to be cleared and LowRate optimize bit to be checked
        setSignalBandwidth(125);
        
        setLnaGain(0);          // AGC Loop control LNA gain
        setTxPaLevel(17, true);

        // RegPayloadLength
        // status |= _writeRegister(Lora_RegIrqFlags, 0xB7); // Only TxDone and RxDone IRQs enabled
        status |= _writeRegister(Lora_RegIrqFlags, ~(IrqTxDone | IrqRxDone)); // Only TxDone and RxDone IRQs enabled

        status |= _writeRegister(Lora_RegFifoTxBaseAddr, TxBase);             // Set TX FIFO base addr to 0x80
        status |= _writeRegister(Lora_RegFifoRxBaseAddr, RxBase);             // Set RX FIFO base addr to 0x00

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

    bool Lora::transmitBusy()
    {
        if (ModeTransmit == (_readRegister(RegOpMode) & ModeTransmit))
        {
            return true;
        }

        return false;
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

    esp_err_t Lora::transmitString(const uint8_t *data_tx)
    {
        esp_err_t status{ESP_OK};
        // TODO Set FifoPtrAddr to FifoTxPtrBase.
        // TODO Write PayloadLength bytes to the FIFO (RegFifo)
        // TODO Set Mode to TX
        // TODO Check and handle TX done interrupt

        return status;
    }

    esp_err_t Lora::transmitByte(const uint8_t data_tx)
    {
        esp_err_t status{ESP_OK};
        _setInterruptTxRx(tx_int);
        status |= _writeRegister(RegOpMode, ModeLongRangeMode | ModeStandby);   // Set to Standby
        // Set FifoPtrAddr to FifoTxPtrBase.
        
        _writeRegister(Lora_RegFifoAddrPtr, TxBase);               // Set FIFO ptr to TX Base
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


        status |= _writeRegister(Lora_RegFifoAddrPtr, RxBase);               // Rx FIFO start at 0x00
        status |= _writeRegister(Lora_RegPayloadLength, 0x01);             // Payload Length is 1 byte
        status |= _writeRegister(RegOpMode, ModeLongRangeMode | ModeReceiveContinuous);   // Set to RX Continuous mode

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
        return _readRegister(RegFifo);
    }

    esp_err_t Lora::standBy(void)
    {
        return _writeRegister(RegOpMode, ModeLongRangeMode | ModeStandby); 
    }

    esp_err_t Lora::sleep(void)
    {
        return _writeRegister(RegOpMode, ModeLongRangeMode | ModeSleep);
    }

    uint8_t Lora::getSpreadingFactor(void)
    {
        return (_readRegister(Lora_RegModemConfig2) >> 4);
    }
    
    uint32_t Lora::getSignalBandwidth(void)
    {
        uint8_t bw = (_readRegister(Lora_RegModemConfig1) >> 4);

        switch(bw)
        {
            case 0: return 7.8E3;
            case 1: return 10.4E3;
            case 2: return 15.6E3;
            case 3: return 20.8E3;
            case 4: return 31.25E3;
            case 5: return 41.7E3;
            case 6: return 62.5E3;
            case 7: return 125E3;
            case 8: return 250E3;
            case 9: return 500E3; 
        }

        return 0;
    }

    esp_err_t Lora::lowDataRateOptimize(void)
    {
        // LowDataRateOptimize, page 27, 28, 114
        // bw / 2^sf seconds... 1000 / (bw / 2^sf) = ms
        uint32_t symbolLength = 1000 / (getSignalBandwidth() / (1L << getSpreadingFactor()));

        if(symbolLength > 16)
        {
            return _writeRegister(Lora_RegModemConfig3, _readRegister(Lora_RegModemConfig3) | (0x01 << 3));
        }
        else
        {
            return _writeRegister(Lora_RegModemConfig3, _readRegister(Lora_RegModemConfig3) & 0xF7);
        }
    }

    esp_err_t Lora::setSpreadingFactor(uint8_t sf)
    {
        esp_err_t status{ESP_OK};

        if (sf <= 6)
        {
            sf = 6;
            status |= _writeRegister(Lora_RegDetectOptimize, 0x05);        
            status |= _writeRegister(Lora_RegDetection_Threshold, 0x0C);
        }
        else
        {
            if (sf > 12) { sf = 12; }
            status |= _writeRegister(Lora_RegDetectOptimize, 0x03);        
            status |= _writeRegister(Lora_RegDetection_Threshold, 0x0A);
        }

        status |= _writeRegister(Lora_RegModemConfig2, (_readRegister(Lora_RegModemConfig2) & 0x0F) | ((sf << 4) & 0xF0));

        lowDataRateOptimize();

        return status;
    }

    esp_err_t Lora::setSignalBandwidth(const uint32_t bw)
    {
        esp_err_t status{ESP_OK};

        uint8_t rbw = 7;

        if (bw <= 7.8E3) { rbw = 0; }
        else if (bw <= 10.4E3) { rbw = 1; }
        else if (bw <= 15.6E3) { rbw = 2; }
        else if (bw <= 20.8E3) { rbw = 3; }
        else if (bw <= 31.25E3) { rbw = 4; }
        else if (bw <= 41.7E3) { rbw = 5; }
        else if (bw <= 62.5E3) { rbw = 6; }
        else if (bw <= 125E3) { rbw = 7; }
        else if (_frequency > 169E6)        // channel 8 and 9 not supported on frequencies below 169Mhz
        {
            if (bw <= 250E3) { rbw = 8; }
            else { rbw = 9; }
        }

        if (9 == rbw)
        {
            status |= _writeRegister(0x36, 0x02);     // Erreta Note page 4
            status |= _writeRegister(0x3A, 0x7F);     // Erreta Note page 4
        }
        else
        {
            status |= _writeRegister(0x36, 0x03);     // Erreta Note page 4
        }                                   // Remove this is it causes issues

        status |= _writeRegister(Lora_RegModemConfig1, (_readRegister(Lora_RegModemConfig1) & 0x0F) | ((rbw << 4) & 0xF0));

        lowDataRateOptimize();

        return status;
    }

    esp_err_t Lora::setCodingRate(uint8_t cr_denominator)
    {
        if (cr_denominator < 5) { cr_denominator = 5; }
        else 
        if (cr_denominator > 8) { cr_denominator = 8; }

        return _writeRegister(Lora_RegModemConfig1, (_readRegister(Lora_RegModemConfig1) & 0xF1) | (((cr_denominator - 4) << 1) & 0x0E));
    }

    esp_err_t Lora::enablePayloadCrc(void)
    {
        return _writeRegister(Lora_RegModemConfig2, _readRegister(Lora_RegModemConfig2) | 0x04);
    }

    esp_err_t Lora::disablePayloadCrc(void)
    {
        return _writeRegister(Lora_RegModemConfig2, _readRegister(Lora_RegModemConfig2) & 0xFB);
    }    

    esp_err_t Lora::implicitHeaderMode(void)
    {
        _implicit_header_mode = true;

        return _writeRegister(Lora_RegModemConfig1, _readRegister(Lora_RegModemConfig1) | 0x01);
    }

    esp_err_t Lora::explicitHeaderMode(void)
    {
        _implicit_header_mode = false;

        return _writeRegister(Lora_RegModemConfig1, _readRegister(Lora_RegModemConfig1) & 0xFE);
    }

    esp_err_t Lora::clearIrqFlags()
    {
        esp_err_t status{ESP_OK};

        status |= _writeRegister(Lora_RegFifoAddrPtr, 0x00);                 // Reset FIFO pointer to 0
        status |= _writeRegister(Lora_RegIrqFlags, IrqTxDone | IrqRxDone | IrqCrcError);   // Clear IRQ flags

        return status;
    }

    esp_err_t Lora::setPreambleLength(uint16_t length)
    {
        esp_err_t status{ESP_OK};

        status |= _writeRegister(Lora_RegPreambleMsb, static_cast<uint8_t>(length >> 8));
        status |= _writeRegister(Lora_RegPreambleLsb, static_cast<uint8_t>(length >> 0));

        return status;
    }

    esp_err_t Lora::setSyncWord(uint8_t sw)
    {
        return _writeRegister(Lora_RegSyncWord, sw);
    }

    esp_err_t Lora::enableInvertIQ()
    {
        esp_err_t status{ESP_OK};

        status |= _writeRegister(Lora_RegInvertIQ, 0x67);
        status |= _writeRegister(Lora_RegInvertIQ2, 0x19);

        return status;
    }

    esp_err_t Lora::disableInvertIQ()
    {
        esp_err_t status{ESP_OK};

        status |= _writeRegister(Lora_RegInvertIQ, (0x13 << 1));
        status |= _writeRegister(Lora_RegInvertIQ2, 0x1D);

        return status;
    }

    // RX Inverted, TX normal
    esp_err_t Lora::rxInvertIQ()
    {
        esp_err_t status{ESP_OK};

        status |= _writeRegister(Lora_RegInvertIQ, 0x66);
        status |= _writeRegister(Lora_RegInvertIQ2, 0x19);

        return status;
    }

    // RX Inverted, TX normal.
    esp_err_t Lora::txInvertIQ()
    {
        esp_err_t status{ESP_OK};

        status |= _writeRegister(Lora_RegInvertIQ, 0x27);
        status |= _writeRegister(Lora_RegInvertIQ2, 0x1D);

        return status;
    }

    esp_err_t Lora::enableAgcAutoOn(void)
    {
        return _writeRegister(Lora_RegModemConfig3, (_readRegister(Lora_RegModemConfig3) | (AgcLoop << 2)));
    }

    esp_err_t Lora::disableAgcAutoOn(void)
    {
        return _writeRegister(Lora_RegModemConfig3, (_readRegister(Lora_RegModemConfig3) & 0xFB));
    }

    esp_err_t Lora::setLnaGain(uint8_t lnaGain)
    {
        esp_err_t status{ESP_OK};

        if (lnaGain > 6) { lnaGain = 6; }

        status |= _writeRegister(lnaGain, 0x00);      // Disable LNA boost and get values into a known state

        if (0 == lnaGain)
        {
            status |= enableAgcAutoOn();
        }
        else
        {
            status |= disableAgcAutoOn();
            if (_frequency >= 779E6)            // Is frequency in HF band
            {
                 status |= _writeRegister(LnaGain, 0x03); // Boost LNA current for HF band
            }
        }
        
        status |= _writeRegister(LnaGain, _readRegister(LnaGain) | lnaGain << 5);

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

    // TODO Test frequency and set correct offset
    int Lora::getRSSI(void)
    {
        return static_cast<int>(_readRegister(Lora_RegRssiValue)) - 164;
    }

    // TODO Test frequency and set correct offset
    int Lora::getPacketRSSI(void)
    {
        return static_cast<int>(_readRegister(Lora_RegPktRssiValue)) - 164;
    }

    bool Lora::crcError(void)
    {
        return static_cast<bool>(_readRegister(Lora_RegIrqFlags) & IrqCrcError);
    }

} // namespace Lora