#pragma once

#include <string>
#include <iostream>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

#include "Gpio.h"
#include "Wifi.h"
#include "Lora.h"
#include "SntpTime.h"
#include "AppTimer.h"

class Main final
{
private:
    constexpr static const char *AppTimer_tag{"AppTimer"};
    constexpr static const char *Lora_tag{"Lora"};

public:
    Main(void) : SntpTime {SNTP::Sntp::get_instance()} {}

    constexpr static int spi_3_miso = 19;
    constexpr static int spi_3_mosi = 23;
    constexpr static int spi_3_sclk = 18;

    constexpr static int lora_ss_pin = 5;

    constexpr static gpio_num_t lora_reset_pin = GPIO_NUM_4;

    esp_err_t setup(void);
    void run(void);

    static void apptimer_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data);
    static void lora_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data);

    WIFI::Wifi::state_e wifiState { WIFI::Wifi::state_e::NOT_INITIALISED };

    Gpio::GpioOutput led1Green { GPIO_NUM_14 };
    Gpio::GpioOutput led1Red { GPIO_NUM_25 };
    Gpio::GpioOutput LoraLedGreen { GPIO_NUM_26 };
    Gpio::GpioOutput LoraLedRed { GPIO_NUM_27 };
    WIFI::Wifi Wifi;
    SPI::Spi Spi_3;
    LORA::Lora Lora;
    SNTP::Sntp& SntpTime;
    TIMER::AppTimer AppTimer;
};

static Main App;