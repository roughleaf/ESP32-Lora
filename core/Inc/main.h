#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "Gpio.h"
#include "Wifi.h"
#include "Lora.h"
#include <string>
#include <iostream>

class Main final
{
public:
    spi_bus_config_t spi_cfg{   // Move this to a dedicated SPI class, its late I want to finish this and go to bed.
        .mosi_io_num = 23,
        .miso_io_num = 19,
        .sclk_io_num = 18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 0,        
        .flags = 0,
        .intr_flags = 0 };

    esp_err_t setup(void);
    void run(void);

    Gpio::GpioOutput led { GPIO_NUM_14 };
    WIFI::Wifi testMac;
    Lora::Lora LoraDev;
};