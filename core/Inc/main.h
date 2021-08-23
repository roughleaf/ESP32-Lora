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
    constexpr static int spi_3_miso = 19;
    constexpr static int spi_3_mosi = 23;
    constexpr static int spi_3_sclk = 18;

    esp_err_t setup(void);
    void run(void);

    Gpio::GpioOutput led { GPIO_NUM_14 };
    WIFI::Wifi Wifi;
    SPI::Spi Spi_3;
    LORA::Lora Lora;
};