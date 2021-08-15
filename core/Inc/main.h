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
    esp_err_t setup(void);
    void run(void);

    Gpio::GpioOutput led { GPIO_NUM_14 };
    WIFI::Wifi Wifi;
    Lora::Lora LoraDev;
};