#include "Gpio.h"

namespace Gpio
{
     GpioOutput::GpioOutput(const gpio_num_t pin)
    {
        _pin = pin;

        gpio_config_t _cfg{
            .pin_bit_mask = static_cast<uint64_t>(1) << pin,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE};

        gpio_config(&_cfg);
    }

    esp_err_t GpioOutput::High(void)
    {
        return gpio_set_level(_pin, 1);
    }

    esp_err_t GpioOutput::Low(void)
    {
        return gpio_set_level(_pin, 0);
    }

} // namespace Gpio