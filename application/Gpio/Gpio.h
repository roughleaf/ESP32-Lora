#pragma once

#include "driver/gpio.h"

namespace Gpio
{
    class GpioOutput
    {
    private:
        gpio_num_t _pin;
        bool _active_low;

    public:
        GpioOutput(const gpio_num_t pin, const bool activeLow);
        GpioOutput(const gpio_num_t pin);
        esp_err_t High(void);
        esp_err_t Low(void);
        esp_err_t Active(void);
        esp_err_t Inactive(void);

    }; // GpioOutput Class
} // namespace Gpio