#pragma once

#include "driver/gpio.h"

namespace Gpio
{
    class GpioInput
    {
    private:
        gpio_num_t _pin;
        bool _active_low;

        esp_err_t _init (const gpio_num_t pin);

    public:
        GpioInput(const gpio_num_t pin);
        GpioInput(void);
        esp_err_t Init (const gpio_num_t pin);
    }; // GpioInput Class

    class GpioOutput
    {
    private:
        gpio_num_t _pin;
        bool _active_low;
        bool _level = false;

        esp_err_t _init (const gpio_num_t pin, const bool activeLow);

    public:
        GpioOutput(const gpio_num_t pin, const bool activeLow);
        GpioOutput(const gpio_num_t pin);
        GpioOutput(void);
        esp_err_t Init (const gpio_num_t pin, const bool activeLow);
        esp_err_t Init (const gpio_num_t pin);
        esp_err_t On(void);
        esp_err_t Off(void);
        esp_err_t Toggle(void);

    }; // GpioOutput Class
} // namespace Gpio