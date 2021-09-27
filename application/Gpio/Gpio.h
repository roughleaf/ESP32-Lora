#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_event.h"
#include "driver/gpio.h"

namespace Gpio
{
    ESP_EVENT_DECLARE_BASE(INPUT_EVENTS);

    class GpioBase
    {
    protected:
        gpio_num_t _pin;
        bool _active_low;

    }; // GpioBase Class

    class GpioInput : public GpioBase
    {
    private:
        esp_err_t _init (const gpio_num_t pin);
        static bool _event_handler_set;
        static bool _interrupts_enabled;

        
        

    public:
        GpioInput(const gpio_num_t pin);
        GpioInput(void);
        esp_err_t init (const gpio_num_t pin);
        int read(void);
        esp_err_t setEventHandler(esp_event_handler_t Gpio_e_h);
        static esp_err_t enableInterrupt(const gpio_num_t pin);

        static void IRAM_ATTR gpio_isr_callback(void* arg);
    }; // GpioInput Class

    class GpioOutput : public GpioBase
    {
    private:        
        bool _level = false;

        esp_err_t _init (const gpio_num_t pin, const bool activeLow);

    public:
        GpioOutput(const gpio_num_t pin, const bool activeLow);
        GpioOutput(const gpio_num_t pin);
        GpioOutput(void);
        esp_err_t init (const gpio_num_t pin, const bool activeLow);
        esp_err_t init (const gpio_num_t pin);
        esp_err_t on(void);
        esp_err_t off(void);
        esp_err_t toggle(void);

    }; // GpioOutput Class
} // namespace Gpio