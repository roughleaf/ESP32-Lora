#include "Wifi.h"

#define LOG_TAG "WIFI"

namespace WIFI
{
    // Statics
    char Wifi::mac_addr_cstr[]{};
    std::mutex Wifi::first_call_mutx;
    bool Wifi::first_call{true};

    constexpr static const char *ssid{"TestGuest"};
    constexpr static const char *password{"00000000"};

    Wifi::Wifi(void)
    {
        std::lock_guard<std::mutex> guard(first_call_mutx);
        if(first_call)
        {
            if(ESP_OK != _get_mac()) esp_restart();
            first_call = false;
        }
        _wifi_initialise();
    }

    // TODO Major refactor, move config to static classwide scope
    // Add error checking afer each stage.
    // Impliment the state machine
    esp_err_t Wifi::_wifi_initialise()
    {
        esp_err_t status{ESP_OK};
        wifi_init_config_t _cfg = WIFI_INIT_CONFIG_DEFAULT();
        wifi_config_t sta_cfg;

        memcpy(sta_cfg.sta.ssid, ssid, strlen(ssid));
        memcpy(sta_cfg.sta.password, password, strlen(password));

        sta_cfg.sta.threshold.authmode = WIFI_AUTH_OPEN;
        sta_cfg.sta.pmf_cfg.capable = true;
        sta_cfg.sta.pmf_cfg.required = false;

        status |= esp_wifi_init(&_cfg); // Default Values
        ESP_LOGI(LOG_TAG, "Wifi Init");
        status |= esp_wifi_set_mode(WIFI_MODE_STA); // Wifi Station mode
        ESP_LOGI(LOG_TAG, "WiFi STA Mode");
        status |= esp_wifi_set_config(WIFI_IF_STA, &sta_cfg);
        ESP_LOGI(LOG_TAG, "WiFi STA Config");
        status |= esp_wifi_start(); // start Wifi
        ESP_LOGI(LOG_TAG, "WiFi Start");
        status |= esp_wifi_connect();
        ESP_LOGI(LOG_TAG, "WiFi Connect");

        return status;
    }

    esp_err_t Wifi::_get_mac(void)
    {
        uint8_t mac_byte_buffer[6]{};

        const esp_err_t status{esp_efuse_mac_get_default(mac_byte_buffer)};

        if (ESP_OK == status)
        {
            snprintf(mac_addr_cstr, sizeof(mac_addr_cstr), "%02X%02X%02X%02X%02X%02X",
                     mac_byte_buffer[0],
                     mac_byte_buffer[1],
                     mac_byte_buffer[2],
                     mac_byte_buffer[3],
                     mac_byte_buffer[4],
                     mac_byte_buffer[5]);
        }

        return status;
    }

} // namespace WIFI