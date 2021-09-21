#include "main.h"

void Main::apptimer_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
    {
        App.wifiState = App.Wifi.get_state();

        switch (App.wifiState)
        {
        case WIFI::Wifi::state_e::READY_TO_CONNECT:
        case WIFI::Wifi::state_e::DISCONNECTED:
            ESP_LOGI(AppTimer_tag, "Trying to connect");
            App.led1Red.On();
            App.led1Green.Toggle();
            App.Wifi.Begin();
            break;
        case WIFI::Wifi::state_e::CONNECTING:
            App.led1Red.On();
            App.led1Green.Toggle();
            ESP_LOGI(AppTimer_tag, "Trying to connect");
            break;
        case WIFI::Wifi::state_e::WAITING_FOR_IP:
            ESP_LOGI(AppTimer_tag, "Waiting for IP");
            App.led1Red.Toggle();
            App.led1Green.On();
            break;
        case WIFI::Wifi::state_e::ERROR:
            ESP_LOGI(AppTimer_tag, "Error");
            App.led1Red.On();
            App.led1Green.Off();
            break;
        case WIFI::Wifi::state_e::CONNECTED:
            ESP_LOGI(AppTimer_tag, "Connected");
            App.led1Red.Off();
            App.led1Green.On();
            break;
        case WIFI::Wifi::state_e::NOT_INITIALISED:
            ESP_LOGI(AppTimer_tag, "Not Initialised State");
            break;
        case WIFI::Wifi::state_e::INITIALISED:
            ESP_LOGI(AppTimer_tag, "Initialised State");
            break;
        }
    }

    void Main::lora_event_handler(void *handler_args, esp_event_base_t base, int32_t id, void *event_data)
    {
    }