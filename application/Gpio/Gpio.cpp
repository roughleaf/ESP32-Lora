#include "Gpio.h"

namespace Gpio
{
    bool GpioInput::_event_handler_set{false};
    bool GpioInput::_interrupts_enabled{false};
    ESP_EVENT_DEFINE_BASE(INPUT_EVENTS);

// ================================= GpioInput ==============================
    void IRAM_ATTR GpioInput::gpio_isr_callback(void *args)
    {
        if (_event_handler_set)
        {
            esp_event_isr_post(INPUT_EVENTS, 0, nullptr, 0, nullptr);
        }
    }

    esp_err_t GpioInput::_init (const gpio_num_t pin)
    {
        esp_err_t status{ESP_OK};

        _pin = pin;
        gpio_config_t cfg;
        cfg.pin_bit_mask = 1ULL << pin;
        cfg.mode = GPIO_MODE_INPUT;
        cfg.pull_up_en = GPIO_PULLUP_DISABLE;
        cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
        cfg.intr_type = GPIO_INTR_POSEDGE;

        status |= gpio_config(&cfg);

        return status;
    }
    
    GpioInput::GpioInput(const gpio_num_t pin)
    {
        _init(pin);
    }

    GpioInput::GpioInput(void)
    {

    }
    
    esp_err_t GpioInput::Init(const gpio_num_t pin)
    {
        return _init(pin);
    }

    int GpioInput::Read(void)
    {
        return gpio_get_level(_pin);
    }

    esp_err_t GpioInput::SetEventHandler(esp_event_handler_t Gpio_e_h)
    {
        esp_err_t status{ESP_OK};

        status |= esp_event_handler_instance_register(INPUT_EVENTS, 0, Gpio_e_h, 0, nullptr);

        if (ESP_OK == status)
        {
            _event_handler_set = true;
        }

        return status;
    }

    esp_err_t GpioInput::EnableInterrupt(const gpio_num_t pin)
    {
        esp_err_t status {ESP_OK};

        if (!_interrupts_enabled)
        {
            status = gpio_install_isr_service(0);
        }
        if (ESP_OK == status)
        {
            _interrupts_enabled = true;
            status |= gpio_isr_handler_add(pin, gpio_isr_callback, (void*) pin);
        }
        return status;
    }

// ================================= GpioOutput ==============================
    esp_err_t GpioOutput::_init(const gpio_num_t pin, const bool activeLow)
    {
        esp_err_t status{ESP_OK};

        _active_low = activeLow;
        _pin = pin;

        gpio_config_t cfg;
        cfg.pin_bit_mask = 1ULL << pin;
        cfg.mode = GPIO_MODE_OUTPUT;
        cfg.pull_up_en = GPIO_PULLUP_DISABLE;
        cfg.pull_down_en = GPIO_PULLDOWN_DISABLE;
        cfg.intr_type = GPIO_INTR_DISABLE;

        status |= gpio_config(&cfg);

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

    esp_err_t GpioOutput::Init(const gpio_num_t pin)
    {
        return _init(pin, false);
    }

    esp_err_t GpioOutput::On(void)
    {
        _level = true;;
        return gpio_set_level(_pin, _active_low ? false : true);
    }

    esp_err_t GpioOutput::Off(void)
    {
        _level = false;
        return gpio_set_level(_pin, _active_low ? true : false);
    }

    esp_err_t GpioOutput::Toggle(void)
    {
        _level = _level ? false : true;
        return gpio_set_level(_pin, _level ? true : false);
    }

} // namespace Gpio