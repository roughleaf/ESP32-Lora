#include "Gpio.h"

namespace Gpio
{
    esp_err_t GpioOutput::_init(const gpio_num_t pin, const bool activeLow)
    {
        esp_err_t status = ESP_OK;

        _active_low = activeLow;
        _pin = pin;

        gpio_config_t _cfg{
            .pin_bit_mask = static_cast<uint64_t>(1) << pin,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE};

        status |= gpio_config(&_cfg);

        return status;
    }

    GpioOutput::GpioOutput(const gpio_num_t pin, const bool activeLow)
    {
       _init(pin, activeLow);
    }

    GpioOutput::GpioOutput(const gpio_num_t pin)
    {
        _init(pin, false);
    }

    GpioOutput::GpioOutput(void)
    {        
    }

    esp_err_t GpioOutput::Init(const gpio_num_t pin, const bool activeLow)
    {
        return _init(pin, activeLow);
    }

    esp_err_t GpioOutput::On(void)
    {
        _level = true;;
        return gpio_set_level(_pin, 1);
    }

    esp_err_t GpioOutput::Off(void)
    {
        _level = false;
        return gpio_set_level(_pin, 0);
    }

    esp_err_t GpioOutput::Toggle(void)
    {
        _level = _level ? false : true;
        return gpio_set_level(_pin, _level ? true : false);
    }

} // namespace Gpio