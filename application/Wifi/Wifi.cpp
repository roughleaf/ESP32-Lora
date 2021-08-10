#include "Wifi.h"

#define LOG_TAG "WIFI"

namespace WIFI
{
    // Statics
    char                Wifi::mac_addr_cstr[]{};
    std::mutex          Wifi::init_mutx;
    Wifi::state_e       Wifi::_state{state_e::NOT_INITIALISED};
    wifi_init_config_t  Wifi::wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    wifi_config_t       Wifi::wifi_cfg{};

    constexpr static const char *ssid{"TestGuest"};
    constexpr static const char *password{"00000000"};

    // Wifi Contrustor
    Wifi::Wifi(void)
    {
        std::lock_guard<std::mutex> guard(init_mutx);
        if(!mac_addr_cstr[0])
        {
            if(ESP_OK != _get_mac())
            {
                esp_restart();
            }
        }
    }

    esp_err_t Wifi::_init(void)
    {
        std::lock_guard<std::mutex> guard(init_mutx);

        esp_err_t status{ESP_OK};

        if(state_e::NOT_INITIALISED == _state)
        {
            status |= esp_netif_init();
            if (ESP_OK == status)
            {
                const esp_netif_t* const p_netif = esp_netif_create_default_wifi_sta();

                if(!p_netif)
                {
                    status = ESP_FAIL;
                }
            }

            if(ESP_OK == status)
            {
                status = esp_wifi_init(&wifi_init_cfg);
            }

        }
        else if (state_e::ERROR == _state)
        {
            status = ESP_FAIL;
        }

        return status;
    }

    // TODO Major refactor, move config to static classwide scope
    // Add error checking afer each stage.
    // Impliment the state machine
    esp_err_t Wifi::init(void)
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

    // Get default MAC from API and convert to ASCII HEX
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