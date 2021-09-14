#pragma once

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "Gpio.h"
#include "SPI.h"

namespace LORA
{
    // Common Registers
    constexpr static uint8_t RegFifo = 0x00;
    constexpr static uint8_t RegOpMode = 0x01;
    constexpr static uint8_t RegFrfMsb = 0x06;
    constexpr static uint8_t RegFrfMid = 0x07;
    constexpr static uint8_t RegFrfLsb = 0x08;
    constexpr static uint8_t RegPaConfig = 0x09;
    constexpr static uint8_t RegPaRamp = 0x0A;
    constexpr static uint8_t RegOcp = 0x0B;
    constexpr static uint8_t RegLna = 0x0C;
    constexpr static uint8_t RegDioMapping1 = 0x40;
    constexpr static uint8_t RegDioMapping2 = 0x41;
    constexpr static uint8_t RegVersion1 = 0x42;
    constexpr static uint8_t RegTcxo = 0x4B;
    constexpr static uint8_t RegPaDac = 0x4D; 
    constexpr static uint8_t RegAgcRef = 0x61;
    constexpr static uint8_t RegAgcThresh1 = 0x62;
    constexpr static uint8_t RegAgcThresh2 = 0x63;
    constexpr static uint8_t RegAgcThresh3 = 0x64;
    constexpr static uint8_t RegPll = 0x70;

    // FSK/OOK Mode Registers
    constexpr static uint8_t FSK_OOK_RegBitrateMsb = 0x02;
    constexpr static uint8_t FSK_OOK_RegBitrateLsb = 0x03;
    constexpr static uint8_t FSK_OOK_RegFdevMsb = 0x04;
    constexpr static uint8_t FSK_OOK_RegFdevLsb = 0x05;
    constexpr static uint8_t FSK_OOK_RegRxConfig = 0x0D;
    constexpr static uint8_t FSK_OOK_RegRssiConfig = 0x0E;
    constexpr static uint8_t FSK_OOK_RegRssiCollision = 0x0F;
    constexpr static uint8_t FSK_OOK_RegRssiThresh = 0x10;
    constexpr static uint8_t FSK_OOK_RegRssiValue = 0x11;
    constexpr static uint8_t FSK_OOK_RegRxBw = 0x12;
    constexpr static uint8_t FSK_OOK_RegAfcBw = 0x13;
    constexpr static uint8_t FSK_OOK_RegOokPeak = 0x14;
    constexpr static uint8_t FSK_OOK_RegOokFix = 0x15;
    constexpr static uint8_t FSK_OOK_RegOokAvg = 0x16;
    constexpr static uint8_t FSK_OOK_RegAfcFei = 0x1A;
    constexpr static uint8_t FSK_OOK_RegAfcMsb = 0x1B;
    constexpr static uint8_t FSK_OOK_RegAfcLsb = 0x1C;
    constexpr static uint8_t FSK_OOK_RegFeiMsb = 0x1D;
    constexpr static uint8_t FSK_OOK_RegFeiLsb = 0x1E;
    constexpr static uint8_t FSK_OOK_RegPreambleDetect = 0x1F;
    constexpr static uint8_t FSK_OOK_RegRxTimeout1 = 0x20;
    constexpr static uint8_t FSK_OOK_RegRxTimeout2 = 0x21;
    constexpr static uint8_t FSK_OOK_RegRxTimeout3 = 0x22;
    constexpr static uint8_t FSK_OOK_RegRxDelay = 0x23;
    constexpr static uint8_t FSK_OOK_RegOsc = 0x24;
    constexpr static uint8_t FSK_OOK_RegPreambleMsb = 0x25;
    constexpr static uint8_t FSK_OOK_RegPreambleLsb = 0x26;
    constexpr static uint8_t FSK_OOK_RegSyncConfig = 0x27;
    constexpr static uint8_t FSK_OOK_RegSyncValue1 = 0x28;
    constexpr static uint8_t FSK_OOK_RegSyncValue2 = 0x29;
    constexpr static uint8_t FSK_OOK_RegSyncValue3 = 0x2A;
    constexpr static uint8_t FSK_OOK_RegSyncValue4 = 0x2B;
    constexpr static uint8_t FSK_OOK_RegSyncValue5 = 0x2C;
    constexpr static uint8_t FSK_OOK_RegSyncValue6 = 0x2D;
    constexpr static uint8_t FSK_OOK_RegSyncValue7 = 0x2E;
    constexpr static uint8_t FSK_OOK_RegSyncValue8 = 0x2F;
    constexpr static uint8_t FSK_OOK_RegPacketConfig1 = 0x30;
    constexpr static uint8_t FSK_OOK_RegPacketConfig2 = 0x31;
    constexpr static uint8_t FSK_OOK_RegPayloadLength = 0x32;
    constexpr static uint8_t FSK_OOK_RegNodeAdrs = 0x33;
    constexpr static uint8_t FSK_OOK_RegBroadcastAdrs = 0x34;
    constexpr static uint8_t FSK_OOK_RegFifoThresh = 0x35;
    constexpr static uint8_t FSK_OOK_RegSeqConfig1 = 0x36;
    constexpr static uint8_t FSK_OOK_RegSeqConfig2 = 0x37;
    constexpr static uint8_t FSK_OOK_RegTimerResol = 0x38;
    constexpr static uint8_t FSK_OOK_RegTimer1Coef = 0x39;
    constexpr static uint8_t FSK_OOK_RegTimer2Coef = 0x3A;
    constexpr static uint8_t FSK_OOK_RegImageCal = 0x3B;
    constexpr static uint8_t FSK_OOK_RegTemp = 0x3C;
    constexpr static uint8_t FSK_OOK_RegLowBat = 0x3D;
    constexpr static uint8_t FSK_OOK_RegIrqFlags1 = 0x3E;
    constexpr static uint8_t FSK_OOK_RegIrqFlags2 = 0x3F;
    constexpr static uint8_t FSK_OOK_RegPllHop = 0x44;
    constexpr static uint8_t FSK_OOK_RegBitRateFrac = 0x5D;

    // LoRa Mode Registers
    constexpr static uint8_t Lora_RegFifoAddrPtr = 0x0D;
    constexpr static uint8_t Lora_RegFifoTxBaseAddr = 0x0E;
    constexpr static uint8_t Lora_RegFifoRxBaseAddr = 0x0F;
    constexpr static uint8_t Lora_FifoRxCurrentAddr = 0x10;
    constexpr static uint8_t Lora_RegIrqFlagsMask = 0x11;
    constexpr static uint8_t Lora_RegIrqFlags = 0x12;
    constexpr static uint8_t Lora_RegRxNbBytes = 0x13;
    constexpr static uint8_t Lora_RegRxHeaderCnt_ValueMsb = 0x14;
    constexpr static uint8_t Lora_RegRxHeaderCnt_ValueLsb = 0x15;
    constexpr static uint8_t Lora_RegRxPacketCnt_ValueMsb = 0x16;
    constexpr static uint8_t Lora_RegRxPacketCnt_ValueLsb = 0x17;
    constexpr static uint8_t Lora_RegModemStat = 0x18;
    constexpr static uint8_t Lora_RegPktSnrValue = 0x19;
    constexpr static uint8_t Lora_RegPktRssiValue = 0x1A;
    constexpr static uint8_t Lora_RegRssiValue = 0x1B;
    constexpr static uint8_t Lora_RegHopChannel = 0x1C;
    constexpr static uint8_t Lora_RegModemConfig1 = 0x1D;
    constexpr static uint8_t Lora_RegModemConfig2 = 0x1E;
    constexpr static uint8_t Lora_RegSymbTimeoutLsb = 0x1F;
    constexpr static uint8_t Lora_RegPreambleMsb = 0x20;
    constexpr static uint8_t Lora_RegPreambleLsb = 0x21;
    constexpr static uint8_t Lora_RegPayloadLength = 0x22;
    constexpr static uint8_t Lora_RegMaxPayloadLength = 0x23;
    constexpr static uint8_t Lora_RegHopPeriod = 0x24;
    constexpr static uint8_t Lora_RegFifoRxByteAddr = 0x25;
    constexpr static uint8_t Lora_RegModemConfig3 = 0x26;
    constexpr static uint8_t Lora_RegFeiMsb = 0x28;
    constexpr static uint8_t Lora_RegFeiMid = 0x29;
    constexpr static uint8_t Lora_RegFeiLsb = 0x2A;
    constexpr static uint8_t Lora_RegRssiWideband = 0x2C;
    constexpr static uint8_t Lora_RegIfFreq1 = 0x2F;
    constexpr static uint8_t Lora_RegIfFreq2 = 0x30;
    constexpr static uint8_t Lora_RegDetectOptimize = 0x31;
    constexpr static uint8_t Lora_RegInvertIQ = 0x33;
    constexpr static uint8_t Lora_RegHighBwOptimize1 = 0x36;
    constexpr static uint8_t Lora_RegDetection_Threshold = 0x37;
    constexpr static uint8_t Lora_RegSyncWord = 0x39;
    constexpr static uint8_t Lora_RegHighBwOptimize2 = 0x3A;
    constexpr static uint8_t Lora_RegInvertIQ2 = 0x3B;

    class Lora
    {
    private:
        spi_device_handle_t _spi_handle;
        Gpio::GpioOutput _reset;
        SPI::Spi *_spi;

        uint64_t _frequency{};

    public:
        esp_err_t Init(void);
        esp_err_t SetFrequency(uint64_t frequency);
        esp_err_t SpiSetup(SPI::Spi *l_spi, const int ss, gpio_num_t reset_pin);
        uint8_t ReadRegister(uint8_t reg_addr);
        esp_err_t WriteRegister(uint8_t reg_addr, uint8_t reg_data);
        esp_err_t TransmitString(const char *data_tx);

    }; // class Lora
} // namespace Lora