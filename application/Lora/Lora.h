#pragma once

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "Gpio.h"
#include "SPI.h"

namespace LORA
{
    // Common Registers
    constexpr static const uint8_t RegFifo = 0x00;
    constexpr static const uint8_t RegOpMode = 0x01;
    constexpr static const uint8_t RegFrfMsb = 0x06;
    constexpr static const uint8_t RegFrfMid = 0x07;
    constexpr static const uint8_t RegFrfLsb = 0x08;
    constexpr static const uint8_t RegPaConfig = 0x09;
    constexpr static const uint8_t RegPaRamp = 0x0A;
    constexpr static const uint8_t RegOcp = 0x0B;
    constexpr static const uint8_t RegLna = 0x0C;
    constexpr static const uint8_t RegDioMapping1 = 0x40;
    constexpr static const uint8_t RegDioMapping2 = 0x41;
    constexpr static const uint8_t RegVersion1 = 0x42;
    constexpr static const uint8_t RegTcxo = 0x4B;
    constexpr static const uint8_t RegPaDac = 0x4D; 
    constexpr static const uint8_t RegAgcRef = 0x61;
    constexpr static const uint8_t RegAgcThresh1 = 0x62;
    constexpr static const uint8_t RegAgcThresh2 = 0x63;
    constexpr static const uint8_t RegAgcThresh3 = 0x64;
    constexpr static const uint8_t RegPll = 0x70;

    // FSK/OOK Mode Registers
    constexpr static const uint8_t FSK_OOK_RegBitrateMsb = 0x02;
    constexpr static const uint8_t FSK_OOK_RegBitrateLsb = 0x03;
    constexpr static const uint8_t FSK_OOK_RegFdevMsb = 0x04;
    constexpr static const uint8_t FSK_OOK_RegFdevLsb = 0x05;
    constexpr static const uint8_t FSK_OOK_RegRxConfig = 0x0D;
    constexpr static const uint8_t FSK_OOK_RegRssiConfig = 0x0E;
    constexpr static const uint8_t FSK_OOK_RegRssiCollision = 0x0F;
    constexpr static const uint8_t FSK_OOK_RegRssiThresh = 0x10;
    constexpr static const uint8_t FSK_OOK_RegRssiValue = 0x11;
    constexpr static const uint8_t FSK_OOK_RegRxBw = 0x12;
    constexpr static const uint8_t FSK_OOK_RegAfcBw = 0x13;
    constexpr static const uint8_t FSK_OOK_RegOokPeak = 0x14;
    constexpr static const uint8_t FSK_OOK_RegOokFix = 0x15;
    constexpr static const uint8_t FSK_OOK_RegOokAvg = 0x16;
    constexpr static const uint8_t FSK_OOK_RegAfcFei = 0x1A;
    constexpr static const uint8_t FSK_OOK_RegAfcMsb = 0x1B;
    constexpr static const uint8_t FSK_OOK_RegAfcLsb = 0x1C;
    constexpr static const uint8_t FSK_OOK_RegFeiMsb = 0x1D;
    constexpr static const uint8_t FSK_OOK_RegFeiLsb = 0x1E;
    constexpr static const uint8_t FSK_OOK_RegPreambleDetect = 0x1F;
    constexpr static const uint8_t FSK_OOK_RegRxTimeout1 = 0x20;
    constexpr static const uint8_t FSK_OOK_RegRxTimeout2 = 0x21;
    constexpr static const uint8_t FSK_OOK_RegRxTimeout3 = 0x22;
    constexpr static const uint8_t FSK_OOK_RegRxDelay = 0x23;
    constexpr static const uint8_t FSK_OOK_RegOsc = 0x24;
    constexpr static const uint8_t FSK_OOK_RegPreambleMsb = 0x25;
    constexpr static const uint8_t FSK_OOK_RegPreambleLsb = 0x26;
    constexpr static const uint8_t FSK_OOK_RegSyncConfig = 0x27;
    constexpr static const uint8_t FSK_OOK_RegSyncValue1 = 0x28;
    constexpr static const uint8_t FSK_OOK_RegSyncValue2 = 0x29;
    constexpr static const uint8_t FSK_OOK_RegSyncValue3 = 0x2A;
    constexpr static const uint8_t FSK_OOK_RegSyncValue4 = 0x2B;
    constexpr static const uint8_t FSK_OOK_RegSyncValue5 = 0x2C;
    constexpr static const uint8_t FSK_OOK_RegSyncValue6 = 0x2D;
    constexpr static const uint8_t FSK_OOK_RegSyncValue7 = 0x2E;
    constexpr static const uint8_t FSK_OOK_RegSyncValue8 = 0x2F;
    constexpr static const uint8_t FSK_OOK_RegPacketConfig1 = 0x30;
    constexpr static const uint8_t FSK_OOK_RegPacketConfig2 = 0x31;
    constexpr static const uint8_t FSK_OOK_RegPayloadLength = 0x32;
    constexpr static const uint8_t FSK_OOK_RegNodeAdrs = 0x33;
    constexpr static const uint8_t FSK_OOK_RegBroadcastAdrs = 0x34;
    constexpr static const uint8_t FSK_OOK_RegFifoThresh = 0x35;
    constexpr static const uint8_t FSK_OOK_RegSeqConfig1 = 0x36;
    constexpr static const uint8_t FSK_OOK_RegSeqConfig2 = 0x37;
    constexpr static const uint8_t FSK_OOK_RegTimerResol = 0x38;
    constexpr static const uint8_t FSK_OOK_RegTimer1Coef = 0x39;
    constexpr static const uint8_t FSK_OOK_RegTimer2Coef = 0x3A;
    constexpr static const uint8_t FSK_OOK_RegImageCal = 0x3B;
    constexpr static const uint8_t FSK_OOK_RegTemp = 0x3C;
    constexpr static const uint8_t FSK_OOK_RegLowBat = 0x3D;
    constexpr static const uint8_t FSK_OOK_RegIrqFlags1 = 0x3E;
    constexpr static const uint8_t FSK_OOK_RegIrqFlags2 = 0x3F;
    constexpr static const uint8_t FSK_OOK_RegPllHop = 0x44;
    constexpr static const uint8_t FSK_OOK_RegBitRateFrac = 0x5D;

    // LoRa Mode Registers
    constexpr static const uint8_t Lora_RegFifoAddrPtr = 0x0D;
    constexpr static const uint8_t Lora_RegFifoTxBaseAddr = 0x0E;
    constexpr static const uint8_t Lora_RegFifoRxBaseAddr = 0x0F;
    constexpr static const uint8_t Lora_FifoRxCurrentAddr = 0x10;
    constexpr static const uint8_t Lora_RegIrqFlagsMask = 0x11;
    constexpr static const uint8_t Lora_RegIrqFlags = 0x12;
    constexpr static const uint8_t Lora_RegRxNbBytes = 0x13;
    constexpr static const uint8_t Lora_RegRxHeaderCnt_ValueMsb = 0x14;
    constexpr static const uint8_t Lora_RegRxHeaderCnt_ValueLsb = 0x15;
    constexpr static const uint8_t Lora_RegRxPacketCnt_ValueMsb = 0x16;
    constexpr static const uint8_t Lora_RegRxPacketCnt_ValueLsb = 0x17;
    constexpr static const uint8_t Lora_RegModemStat = 0x18;
    constexpr static const uint8_t Lora_RegPktSnrValue = 0x19;
    constexpr static const uint8_t Lora_RegPktRssiValue = 0x1A;
    constexpr static const uint8_t Lora_RegRssiValue = 0x1B;
    constexpr static const uint8_t Lora_RegHopChannel = 0x1C;
    constexpr static const uint8_t Lora_RegModemConfig1 = 0x1D;
    constexpr static const uint8_t Lora_RegModemConfig2 = 0x1E;
    constexpr static const uint8_t Lora_RegSymbTimeoutLsb = 0x1F;
    constexpr static const uint8_t Lora_RegPreambleMsb = 0x20;
    constexpr static const uint8_t Lora_RegPreambleLsb = 0x21;
    constexpr static const uint8_t Lora_RegPayloadLength = 0x22;
    constexpr static const uint8_t Lora_RegMaxPayloadLength = 0x23;
    constexpr static const uint8_t Lora_RegHopPeriod = 0x24;
    constexpr static const uint8_t Lora_RegFifoRxByteAddr = 0x25;
    constexpr static const uint8_t Lora_RegModemConfig3 = 0x26;
    constexpr static const uint8_t Lora_RegFeiMsb = 0x28;
    constexpr static const uint8_t Lora_RegFeiMid = 0x29;
    constexpr static const uint8_t Lora_RegFeiLsb = 0x2A;
    constexpr static const uint8_t Lora_RegRssiWideband = 0x2C;
    constexpr static const uint8_t Lora_RegIfFreq1 = 0x2F;
    constexpr static const uint8_t Lora_RegIfFreq2 = 0x30;
    constexpr static const uint8_t Lora_RegDetectOptimize = 0x31;
    constexpr static const uint8_t Lora_RegInvertIQ = 0x33;
    constexpr static const uint8_t Lora_RegHighBwOptimize1 = 0x36;
    constexpr static const uint8_t Lora_RegDetection_Threshold = 0x37;
    constexpr static const uint8_t Lora_RegSyncWord = 0x39;
    constexpr static const uint8_t Lora_RegHighBwOptimize2 = 0x3A;
    constexpr static const uint8_t Lora_RegInvertIQ2 = 0x3B;

    // DIO Modes
    constexpr static const uint8_t Lora_RxDone = 0x00 << 6;
    constexpr static const uint8_t Lora_TxDone = 0x01 << 6;

    // Operation Modes
    constexpr static const uint8_t ModeLongRangeMode = 1 << 7;
    constexpr static const uint8_t ModeAccessShareReg = 1 << 6;
    constexpr static const uint8_t ModeLowFrequencyModeOn = 1 << 3;
    constexpr static const uint8_t ModeSleep = 0x00;
    constexpr static const uint8_t ModeStandby = 0x01;    
    constexpr static const uint8_t ModeTransmit = 0x03;
    constexpr static const uint8_t ModeReceiveContinuous = 0x05;
    constexpr static const uint8_t ModeReceiveSingle = 0x06;

    // PA Config
    constexpr static const uint8_t PaBoostPin = 0x80;

    // IRQ Masks
    constexpr static const uint8_t IrqTxDone = 0x08;
    constexpr static const uint8_t IrqCrcError = 0x20;
    constexpr static const uint8_t IrqRxDone = 0x40;

    // LNA Gain Select
    constexpr static const uint8_t LnaGain = 0;
    constexpr static const uint8_t AgcLoop = 1;

    // FIFO Base ptr
    constexpr static const uint8_t RxBase = 0x00;
    constexpr static const uint8_t TxBase = 0x80;


    class Lora
    {
        public:
        enum lora_interrupt_t {tx_int, rx_int};
    private:
        lora_interrupt_t _dio0;

        spi_device_handle_t _spi_handle;
        Gpio::GpioOutput _reset;
        Gpio::GpioInput _lora_irq;
        SPI::Spi *_spi;
        esp_err_t _setInterruptTxRx(lora_interrupt_t dio0);
        esp_err_t _setFrequency(uint64_t frequency);
        uint8_t _readRegister(uint8_t reg_addr);
        esp_err_t _writeRegister(uint8_t reg_addr, uint8_t reg_data);
        uint8_t getSpreadingFactor(void);
        uint32_t getSignalBandwidth(void);
        esp_err_t lowDataRateOptimize(void);
        esp_err_t enableAgcAutoOn(void);
        esp_err_t disableAgcAutoOn(void);
        bool transmitBusy();

        uint64_t _frequency{};
        static bool _irq_enabled;
        bool _implicit_header_mode {false};

    public:
        esp_err_t init(uint64_t frequency);
        esp_err_t spiSetup(SPI::Spi *l_spi, const int ss, gpio_num_t reset_pin);
        esp_err_t irqSetup(const gpio_num_t irq_pin, esp_event_handler_t lora_e_h);
        void irqEnable(bool irq_enabled);
        esp_err_t transmitString(const uint8_t *data_tx);
        esp_err_t transmitByte(const uint8_t data_tx);
        esp_err_t clearIrqFlags();
        esp_err_t listen(void);
        esp_err_t setTxPaLevel(uint8_t pa_level, bool pa_boost);
        esp_err_t setOCP(uint8_t current);
        esp_err_t standBy(void);
        esp_err_t sleep(void);
        esp_err_t setSpreadingFactor(uint8_t sf);
        esp_err_t setSignalBandwidth(const uint32_t bw);
        esp_err_t setCodingRate(uint8_t cr_denominator);
        esp_err_t enablePayloadCrc(void);
        esp_err_t disablePayloadCrc(void);
        esp_err_t implicitHeaderMode(void);
        esp_err_t explicitHeaderMode(void);
        esp_err_t setLnaGain(uint8_t lnaGain);
        int getRSSI(void);
        int getPacketRSSI(void);
        uint8_t readReceivedByte(void);

        esp_err_t setPreambleLength(uint16_t length);
        esp_err_t setSyncWord(uint8_t sw);
        esp_err_t enableInvertIQ(void);
        esp_err_t disableInvertIQ(void);
        esp_err_t rxInvertIQ(void);
        esp_err_t txInvertIQ(void);

        lora_interrupt_t getInterruptMode(void);

        spi_device_handle_t getSpiHandle(void);

        bool crcError(void);


    }; // class Lora
} // namespace Lora