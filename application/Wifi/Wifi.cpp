#include "Wifi.h"

namespace WIFI
{
    Wifi::Wifi(void)
    {
        _get_mac();
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

        memcpy(sta_cfg.sta.ssid, ssid, strlen(ssid)-1);
        memcpy(sta_cfg.sta.password, password, strlen(password)-1);

        sta_cfg.sta.threshold.authmode = WIFI_AUTH_OPEN;
        sta_cfg.sta.pmf_cfg.capable = true;
        sta_cfg.sta.pmf_cfg.required = false;


        status |= esp_wifi_init(&_cfg); // Default Values
        status |= esp_wifi_set_mode(WIFI_MODE_STA);         // Wifi Station mode
        status |= esp_wifi_set_config(WIFI_IF_STA , &sta_cfg);
        status |= esp_wifi_start();                         // start Wifi
        status |= esp_wifi_connect();

        return status;
    }

    char Wifi::mac_addr_cstr[]{};

    esp_err_t Wifi::_get_mac(void)
    {
        uint8_t mac_byte_buffer[6]{};

        const esp_err_t status {esp_efuse_mac_get_default(mac_byte_buffer)};

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