#pragma once

#include "driver/gpio.h"

namespace Gpio
{
    class GpioOutput
    {
    private:
        gpio_num_t _pin;

    public:
        GpioOutput(const gpio_num_t pin);
        esp_err_t High(void);
        esp_err_t Low(void);

    }; // GpioOutput Class
} // namespace Gpio