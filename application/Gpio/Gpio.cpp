#include "Gpio.h"

namespace Gpio
{
    esp_err_t GpioOutput::Init(const gpio_num_t pin)
    {
        _cfg.pin_bit_mask = static_cast<uint64_t>(1) << pin;
        _cfg.mode = GPIO_MODE_OUTPUT;
        _cfg.intr_type = GPIO_INTR_DISABLE;
        _cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
        _cfg.pull_up_en = GPIO_PULLUP_DISABLE;
        return gpio_config(&_cfg);
    }

    esp_err_t GpioOutput::High(void)
    {
        esp_err_t status{ESP_OK};

        status |= gpio_set_level(_pin, true);

        return status;
    }

    esp_err_t GpioOutput::Low(void)
    {
        esp_err_t status{ESP_OK};

        status |= gpio_set_level(_pin, false);

        return status;
    }

} // namespace GPIO