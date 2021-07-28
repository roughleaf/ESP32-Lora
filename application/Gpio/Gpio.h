#pragma once

#include "driver/gpio.h"

namespace Gpio
{
    class GpioOutput
    {
    private:
        gpio_config_t _cfg;
        gpio_num_t _pin;

    public:
        esp_err_t Init(const gpio_num_t pin);
        esp_err_t High(void);
        esp_err_t Low(void);
    }; // GpioBase Class

}